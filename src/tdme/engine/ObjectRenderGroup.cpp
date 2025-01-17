#include <tdme/engine/ObjectRenderGroup.h>

#include <memory>
#include <string>
#include <vector>

#include <tdme/tdme.h>
#include <tdme/engine/model/Face.h>
#include <tdme/engine/model/FacesEntity.h>
#include <tdme/engine/model/Material.h>
#include <tdme/engine/model/Model.h>
#include <tdme/engine/model/Node.h>
#include <tdme/engine/model/RotationOrder.h>
#include <tdme/engine/model/UpVector.h>
#include <tdme/engine/primitives/BoundingBox.h>
#include <tdme/engine/Engine.h>
#include <tdme/engine/LODObject.h>
#include <tdme/engine/Object.h>
#include <tdme/engine/Partition.h>
#include <tdme/engine/Transform.h>
#include <tdme/math/Math.h>
#include <tdme/math/Matrix4x4.h>
#include <tdme/math/Vector2.h>
#include <tdme/math/Vector3.h>
#include <tdme/utilities/ModelTools.h>

using std::make_unique;
using std::string;
using std::to_string;
using std::unique_ptr;
using std::vector;

using tdme::engine::model::Face;
using tdme::engine::model::FacesEntity;
using tdme::engine::model::Material;
using tdme::engine::model::Model;
using tdme::engine::model::Node;
using tdme::engine::model::RotationOrder;
using tdme::engine::model::UpVector;
using tdme::engine::primitives::BoundingBox;
using tdme::engine::Engine;
using tdme::engine::LODObject;
using tdme::engine::Object;
using tdme::engine::ObjectRenderGroup;
using tdme::engine::Partition;
using tdme::engine::Transform;
using tdme::math::Math;
using tdme::math::Matrix4x4;
using tdme::math::Vector2;
using tdme::math::Vector3;
using tdme::utilities::ModelTools;

ObjectRenderGroup::ObjectRenderGroup(
	const string& id,
	int lodLevels,
	float modelLOD2MinDistance,
	float modelLOD3MinDistance,
	int modelLOD2ReduceBy,
	int modelLOD3ReduceBy,
	bool optimizeModels
):
	id(id)
{
	this->enabled = true;
	this->pickable = false;
	this->contributesShadows = false;
	this->receivesShadows = false;
	this->effectColorMul.set(1.0f, 1.0f, 1.0f, 1.0f);
	this->effectColorAdd.set(0.0f, 0.0f, 0.0f, 0.0f);
	this->entityTransformMatrix.identity();
	this->combinedModels.resize(Math::clamp(lodLevels, 1, 3));
	this->combinedEntity = nullptr;
	this->lodReduceBy[0] = 1;
	this->lodReduceBy[1] = modelLOD2ReduceBy;
	this->lodReduceBy[2] = modelLOD3ReduceBy;
	this->modelLOD2MinDistance = modelLOD2MinDistance;
	this->modelLOD3MinDistance = modelLOD3MinDistance;
	this->optimizeModels = optimizeModels;
}

ObjectRenderGroup::~ObjectRenderGroup() {
}

void ObjectRenderGroup::combineNode(Node* sourceNode, const vector<Vector3>& origins, const vector<Matrix4x4>& objectParentTransformMatrices, Model* combinedModel) {
	// create node in combined model
	auto combinedModelNode = combinedModel->getNodeById(sourceNode->getId());
	if (combinedModelNode == nullptr) {
		auto newCombinedModelNode = make_unique<Node>(
			combinedModel,
			sourceNode->getParentNode() == nullptr?nullptr:combinedModel->getNodeById(sourceNode->getParentNode()->getId()),
			sourceNode->getId(),
			sourceNode->getName()
		);
		if (sourceNode->getParentNode() == nullptr) {
			combinedModel->getSubNodes()[newCombinedModelNode->getId()] = newCombinedModelNode.get();
		} else {
			combinedModelNode->getParentNode()->getSubNodes()[newCombinedModelNode->getId()] = newCombinedModelNode.get();
		}
		combinedModel->getNodes()[newCombinedModelNode->getId()] = newCombinedModelNode.get();
		//
		combinedModelNode = newCombinedModelNode.release();
	}

	{
		auto sourceNodeVerticesSize = sourceNode->getVertices().size();
		auto sourceNodeNormalsSize = sourceNode->getNormals().size();
		auto sourceNodeTextureCoordinatesSize = sourceNode->getTextureCoordinates().size();
		auto sourceNodeTangentsSize = sourceNode->getTangents().size();
		auto sourceNodeBitangentsSize = sourceNode->getBitangents().size();

		// vertices and such from new model
		auto combinedModelNodeVertices = combinedModelNode->getVertices();
		auto combinedModelNodeNormals = combinedModelNode->getNormals();
		auto combinedModelNodeTextureCoordinates = combinedModelNode->getTextureCoordinates();
		auto combinedModelNodeTangents = combinedModelNode->getTangents();
		auto combinedModelNodeBitangents = combinedModelNode->getBitangents();
		auto combinedModelNodeFacesEntities = combinedModelNode->getFacesEntities();
		auto combinedModelNodeOrigins = combinedModelNode->getOrigins();

		// current indices
		auto combinedModelNodeVerticesIdxStart = combinedModelNodeVertices.size();
		auto combinedModelNodeNormalsIdxStart = combinedModelNodeNormals.size();
		auto combinedModelNodeTextureCoordinatesIdxStart = combinedModelNodeTextureCoordinates.size();
		auto combinedModelNodeTangentsIdxStart = combinedModelNodeTangents.size();
		auto combinedModelNodeBitangentsIdxStart = combinedModelNodeBitangents.size();

		// add vertices and such from source node to new node
		{
			auto i = 0;
			for (const auto& objectParentTransformMatrix: objectParentTransformMatrices) {
				Matrix4x4 transformMatrix;
				transformMatrix.set(sourceNode->getTransformMatrix());
				transformMatrix.multiply(objectParentTransformMatrix);

				//
				for (const auto& vertex: sourceNode->getVertices()) {
					combinedModelNodeOrigins.push_back(origins[i]);
					combinedModelNodeVertices.push_back(transformMatrix.multiply(vertex));
				}
				for (const auto& normal: sourceNode->getNormals()) {
					combinedModelNodeNormals.push_back(transformMatrix.multiplyNoTranslation(normal));
				}
				for (const auto& textureCoordinate: sourceNode->getTextureCoordinates()) {
					combinedModelNodeTextureCoordinates.push_back(textureCoordinate);
				}
				for (const auto& tangent: sourceNode->getTangents()) {
					combinedModelNodeTangents.push_back(transformMatrix.multiplyNoTranslation(tangent));
				}
				for (const auto& bitangent: sourceNode->getBitangents()) {
					combinedModelNodeBitangents.push_back(transformMatrix.multiplyNoTranslation(bitangent));
				}

				//
				i++;
			}
		}

		// add source node faces to new new faces entity
		for (const auto& facesEntity: sourceNode->getFacesEntities()) {
			bool haveTextureCoordinates = facesEntity.isTextureCoordinatesAvailable();
			bool haveTangentsBitangents = facesEntity.isTangentBitangentAvailable();

			// get faces entity
			FacesEntity* combinedModelNodeFacesEntity = nullptr;
			for (auto& combinedModelNodeFacesEntityExisting: combinedModelNodeFacesEntities) {
				if (combinedModelNodeFacesEntityExisting.getId() == facesEntity.getId()) {
					combinedModelNodeFacesEntity = &combinedModelNodeFacesEntityExisting;
					break;
				}
			}
			// create
			if (combinedModelNodeFacesEntity == nullptr) {
				auto newFacesEntity = FacesEntity(
					combinedModelNode,
					facesEntity.getId()
				);
				combinedModelNodeFacesEntities.push_back(newFacesEntity);
				combinedModelNodeFacesEntity = &combinedModelNodeFacesEntities[combinedModelNodeFacesEntities.size() - 1];
				auto combinedModelNodeFacesEntityMaterial = combinedModel->getMaterials()[facesEntity.getMaterial()->getId()];
				if (combinedModelNodeFacesEntityMaterial == nullptr) {
					combinedModelNodeFacesEntityMaterial = ModelTools::cloneMaterial(facesEntity.getMaterial());
					combinedModel->getMaterials()[combinedModelNodeFacesEntityMaterial->getId()] = combinedModelNodeFacesEntityMaterial;
				}
				combinedModelNodeFacesEntity->setMaterial(combinedModelNodeFacesEntityMaterial);
			}

			//
			auto combinedModelNodeFaces = combinedModelNodeFacesEntity->getFaces();

			//
			auto combinedModelNodeVerticesIdx = combinedModelNodeVerticesIdxStart;
			auto combinedModelNodeNormalsIdx = combinedModelNodeNormalsIdxStart;
			auto combinedModelNodeTextureCoordinatesIdx = combinedModelNodeTextureCoordinatesIdxStart;
			auto combinedModelNodeTangentsIdx = combinedModelNodeTangentsIdxStart;
			auto combinedModelNodeBitangentsIdx = combinedModelNodeBitangentsIdxStart;
			for (const auto& objectParentTransformMatrix: objectParentTransformMatrices) {
				// add faces
				for (const auto& face: facesEntity.getFaces()) {
					// get face vertices and such
					const auto& faceVertexIndices = face.getVertexIndices();
					const auto& faceNormalIndices = face.getNormalIndices();
					const auto& faceTextureCoordinatesIndices = face.getTextureCoordinateIndices();
					const auto& faceTangentIndices = face.getTangentIndices();
					const auto& faceBitangentIndices = face.getBitangentIndices();

					//
					auto combinedModelNodeFace =
						Face(
							combinedModelNode,
							combinedModelNodeVerticesIdx + faceVertexIndices[0],
							combinedModelNodeVerticesIdx + faceVertexIndices[1],
							combinedModelNodeVerticesIdx + faceVertexIndices[2],
							combinedModelNodeNormalsIdx + faceNormalIndices[0],
							combinedModelNodeNormalsIdx + faceNormalIndices[1],
							combinedModelNodeNormalsIdx + faceNormalIndices[2]
						);
					if (haveTextureCoordinates == true) {
						combinedModelNodeFace.setTextureCoordinateIndices(
							combinedModelNodeTextureCoordinatesIdx + faceTextureCoordinatesIndices[0],
							combinedModelNodeTextureCoordinatesIdx + faceTextureCoordinatesIndices[1],
							combinedModelNodeTextureCoordinatesIdx + faceTextureCoordinatesIndices[2]
						);
					}
					if (haveTangentsBitangents == true) {
						combinedModelNodeFace.setTangentIndices(
							combinedModelNodeTangentsIdx + faceTangentIndices[0],
							combinedModelNodeTangentsIdx + faceTangentIndices[1],
							combinedModelNodeTangentsIdx + faceTangentIndices[2]
						);
						combinedModelNodeFace.setBitangentIndices(
							combinedModelNodeBitangentsIdx + faceBitangentIndices[0],
							combinedModelNodeBitangentsIdx + faceBitangentIndices[1],
							combinedModelNodeBitangentsIdx + faceBitangentIndices[2]
						);
					}
					combinedModelNodeFaces.push_back(combinedModelNodeFace);
				}

				//
				combinedModelNodeVerticesIdx+= sourceNodeVerticesSize;
				combinedModelNodeNormalsIdx+= sourceNodeNormalsSize;
				combinedModelNodeTextureCoordinatesIdx+= sourceNodeTextureCoordinatesSize;
				combinedModelNodeTangentsIdx+= sourceNodeTangentsSize;
				combinedModelNodeBitangentsIdx+= sourceNodeBitangentsSize;
			}
			combinedModelNodeFacesEntity->setFaces(combinedModelNodeFaces);
		}

		// store back to model
		combinedModelNode->setVertices(combinedModelNodeVertices);
		combinedModelNode->setNormals(combinedModelNodeNormals);
		combinedModelNode->setTextureCoordinates(combinedModelNodeTextureCoordinates);
		combinedModelNode->setTangents(combinedModelNodeTangents);
		combinedModelNode->setBitangents(combinedModelNodeBitangents);
		combinedModelNode->setFacesEntities(combinedModelNodeFacesEntities);
		combinedModelNode->setOrigins(combinedModelNodeOrigins);
	}

	// do child nodes
	for (const auto& [subNodeId, subNode]: sourceNode->getSubNodes()) {
		combineNode(subNode, origins, objectParentTransformMatrices, combinedModel);
	}
}

void ObjectRenderGroup::combineObjects(Model* model, const vector<Transform>& objectsTransform, Model* combinedModel) {
	vector<Matrix4x4> objectTransformMatrices;
	vector<Vector3> origins;
	for (const auto& objectTransform: objectsTransform) {
		Matrix4x4 transformMatrix;
		transformMatrix.set(model->getImportTransformMatrix());
		transformMatrix.multiply(objectTransform.getTransformMatrix());
		objectTransformMatrices.push_back(transformMatrix);
		origins.push_back(objectTransform.getTranslation());
	}
	for (const auto& [subNodeId, subNode]: model->getSubNodes()) {
		combineNode(subNode, origins, objectTransformMatrices, combinedModel);
	}
}

void ObjectRenderGroup::updateRenderGroup() {
	// dispose old object and combined model
	if (combinedEntity != nullptr) {
		if (engine != nullptr) engine->removeEntityFromLists(combinedEntity.get());
		combinedEntity->dispose();
		combinedEntity = nullptr;
	}

	// combine objects to a new model
	for (auto i = 0; i < combinedModels.size(); i++) {
		combinedModels[i] = make_unique<Model>(
			id + ".o3rg.lod." + to_string(i),
			id + ".o3rg.lod." + to_string(i),
			UpVector::Y_UP,
			RotationOrder::ZYX,
			nullptr
		);
	}

	//
	for (const auto& [model, objectsTransform]: transformByModel) {
		auto lodLevel = 0;
		for (const auto& combinedModel: combinedModels) {
			auto reduceByFactor = lodReduceBy[lodLevel];
			lodLevel++;
			auto objectCount = 0;
			vector<Transform> reducedObjectsTransform;
			for (const auto& objectTransform: objectsTransform) {
				if (objectCount % reduceByFactor != 0) {
					objectCount++;
					continue;
				}
				reducedObjectsTransform.push_back(objectTransform);
				objectCount++;
			}
			combineObjects(model, reducedObjectsTransform, combinedModel.get());
		}
	}

	// create new combined object
	for (const auto& combinedModel: combinedModels) {
		if (combinedModel != nullptr) {
			// post process combined model
			ModelTools::shrinkToFit(combinedModel.get());
			ModelTools::createDefaultAnimation(combinedModel.get(), 0);
			ModelTools::setupJoints(combinedModel.get());
			ModelTools::fixAnimationLength(combinedModel.get());
		}
	}

	// optimize models
	if (optimizeModels == true) {
		for (auto i = 0; i < combinedModels.size(); i++) {
			combinedModels[i] = unique_ptr<Model>(ModelTools::optimizeModel(combinedModels[i].release()));
		}
	}

	// no lod
	if (combinedModels.size() == 1) {
		auto combinedObject = new Object(id + ".o3rg", combinedModels[0].get());
		combinedObject->setParentEntity(this);
		combinedObject->setParentTransform(parentTransform);
		combinedObject->setShader(shaderId);
		combinedObject->setContributesShadows(contributesShadows);
		combinedObject->setReceivesShadows(receivesShadows);
		combinedObject->setEngine(engine);
		combinedObject->shaderParameters = shaderParameters;
		combinedObject->update();
		combinedEntity = unique_ptr<Entity>(combinedObject);
	} else
	// lod
	if (combinedModels.size() > 1) {
		// create object, initialize and
		auto combinedLODObject = new LODObject(
			id + ".o3rg",
			combinedModels[0].get(),
			combinedModels[1] == nullptr?LODObject::LODLEVELTYPE_NONE:LODObject::LODLEVELTYPE_MODEL,
			modelLOD2MinDistance,
			combinedModels[1].get(),
			combinedModels[2] == nullptr?LODObject::LODLEVELTYPE_NONE:LODObject::LODLEVELTYPE_MODEL,
			modelLOD3MinDistance,
			combinedModels[2].get()
		);
		combinedLODObject->setParentEntity(this);
		combinedLODObject->setParentTransform(parentTransform);
		combinedLODObject->setShader(shaderId);
		combinedLODObject->setContributesShadows(contributesShadows);
		combinedLODObject->setReceivesShadows(receivesShadows);
		combinedLODObject->setEngine(engine);
		combinedLODObject->shaderParameters = shaderParameters;
		if (combinedLODObject->objectLOD1 != nullptr) {
			combinedLODObject->objectLOD1->shaderParameters = shaderParameters;
		}
		if (combinedLODObject->objectLOD2 != nullptr) {
			combinedLODObject->objectLOD2->shaderParameters = shaderParameters;
		}
		if (combinedLODObject->objectLOD3 != nullptr) {
			combinedLODObject->objectLOD3->shaderParameters = shaderParameters;
		}
		combinedLODObject->update();
		combinedEntity = unique_ptr<Entity>(combinedLODObject);
	}

	//
	updateBoundingBox();
	update();
}

void ObjectRenderGroup::addObject(Model* model, const Transform& transform) {
	transformByModel[model].push_back(transform);
}

void ObjectRenderGroup::setEngine(Engine* engine)
{
	if (this->engine != nullptr) this->engine->deregisterEntity(this);
	this->engine = engine;
	if (engine != nullptr) engine->registerEntity(this);
	if (combinedEntity != nullptr) combinedEntity->setEngine(engine);
}

void ObjectRenderGroup::setRenderer(RendererBackend* rendererBackend)
{
}

void ObjectRenderGroup::setTransform(const Transform& transform)
{
	Transform::setTransform(transform);
	//
	auto entityTransform = parentTransform * (*this);
	entityTransformMatrix = entityTransform.getTransformMatrix();
	// update world bounding box
	worldBoundingBox.fromBoundingVolumeWithTransformMatrix(&boundingBox, entityTransformMatrix);
	//
	if (combinedEntity != nullptr) combinedEntity->setParentTransform(parentTransform);
	// update object
	if (parentEntity == nullptr && frustumCulling == true && engine != nullptr && enabled == true) engine->partition->updateEntity(this);
}

void ObjectRenderGroup::update()
{
	Transform::update();
	//
	auto entityTransform = parentTransform * (*this);
	entityTransformMatrix = entityTransform.getTransformMatrix();
	// update world bounding box
	worldBoundingBox.fromBoundingVolumeWithTransformMatrix(&boundingBox, entityTransformMatrix);
	//
	if (combinedEntity != nullptr) combinedEntity->setParentTransform(parentTransform);
	// update object
	if (parentEntity == nullptr && frustumCulling == true && engine != nullptr && enabled == true) engine->partition->updateEntity(this);
}

void ObjectRenderGroup::setEnabled(bool enabled)
{
	// return if enable state has not changed
	if (this->enabled == enabled) return;

	// frustum if root entity
	if (parentEntity == nullptr) {
		// frustum culling enabled?
		if (frustumCulling == true) {
			// yeo, add or remove from partition
			if (enabled == true) {
				if (engine != nullptr) engine->partition->addEntity(this);
			} else {
				if (engine != nullptr) engine->partition->removeEntity(this);
			}
		}
	}

	//
	this->enabled = enabled;
}

bool ObjectRenderGroup::isFrustumCulling() {
	return frustumCulling;
}

void ObjectRenderGroup::setFrustumCulling(bool frustumCulling) {
	// check if enabled and engine attached
	if (enabled == true && engine != nullptr) {
		// had frustum culling
		if (this->frustumCulling == true) {
			// yep, remove if set to false now
			if (frustumCulling == false) engine->partition->removeEntity(this);
		} else {
			// yep, add if set to true now
			if (frustumCulling == true) engine->partition->addEntity(this);
		}
	}
	this->frustumCulling = frustumCulling;
	// delegate change to engine
	if (engine != nullptr) engine->updateEntityRegistration(this);
}

void ObjectRenderGroup::dispose()
{
	// delegate to combined object
	if (combinedEntity != nullptr) {
		combinedEntity->dispose();
		combinedEntity = nullptr;
	}
	// disose combined model
	combinedModels.clear();
}

void ObjectRenderGroup::initialize()
{
	if (combinedEntity != nullptr) combinedEntity->initialize();
}

