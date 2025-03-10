#pragma once

#include <array>
#include <vector>

#include <tdme/tdme.h>
#include <tdme/engine/primitives/fwd-tdme.h>
#include <tdme/engine/Transform.h>
#include <tdme/math/fwd-tdme.h>
#include <tdme/math/Matrix4x4.h>
#include <tdme/math/Vector3.h>

using std::array;
using std::vector;

using tdme::engine::primitives::OrientedBoundingBox;
using tdme::engine::Transform;
using tdme::math::Matrix4x4;
using tdme::math::Vector3;

/**
 * Axis aligned bounding box used for frustum, this is not directly connectable with physics engine
 * @author Andreas Drewke
 */
class tdme::engine::primitives::BoundingBox
{

private:
	STATIC_DLL_IMPEXT static const array<int32_t, 3> FACE0_INDICES;
	STATIC_DLL_IMPEXT static const array<int32_t, 3> FACE1_INDICES;
	STATIC_DLL_IMPEXT static const array<int32_t, 3> FACE2_INDICES;
	STATIC_DLL_IMPEXT static const array<int32_t, 3> FACE3_INDICES;
	STATIC_DLL_IMPEXT static const array<int32_t, 3> FACE4_INDICES;
	STATIC_DLL_IMPEXT static const array<int32_t, 3> FACE5_INDICES;
	STATIC_DLL_IMPEXT static const array<int32_t, 3> FACE6_INDICES;
	STATIC_DLL_IMPEXT static const array<int32_t, 3> FACE7_INDICES;
	STATIC_DLL_IMPEXT static const array<int32_t, 3> FACE8_INDICES;
	STATIC_DLL_IMPEXT static const array<int32_t, 3> FACE9_INDICES;
	STATIC_DLL_IMPEXT static const array<int32_t, 3> FACE10_INDICES;
	STATIC_DLL_IMPEXT static const array<int32_t, 3> FACE11_INDICES;
	STATIC_DLL_IMPEXT static const array<array<int32_t,3>,12> facesVerticesIndexes;
	Vector3 min;
	Vector3 max;
	vector<Vector3> vertices;
	Vector3 center;
	Vector3 dimensions;

public:
	/**
	 * Public constructor
	 */
	BoundingBox();

	/**
	 * Public constructor
	 * @param boundingBox bounding box
	 */
	BoundingBox(BoundingBox* boundingBox);

	/**
	 * Public constructor
	 * @param obb oriented bounding box
	 */
	BoundingBox(OrientedBoundingBox* obb);

	/**
	 * Public constructor
	 * @param min min
	 * @param max max
	 */
	BoundingBox(const Vector3& min, const Vector3& max);

	/**
	 * Compute closest point in bounding box of given point
	 * @param point point
	 * @returns closest point of given point in bounding box
	 */
	inline Vector3 computeClosestPointInBoundingBox(const Vector3& point) {
		const auto& pointXYZ = point.getArray();
		const auto& minXYZ = min.getArray();
		const auto& maxXYZ = max.getArray();
		return Vector3(
			pointXYZ[0] < minXYZ[0]?minXYZ[0]:pointXYZ[0] > maxXYZ[0]?maxXYZ[0]:pointXYZ[0],
			pointXYZ[1] < minXYZ[1]?minXYZ[1]:pointXYZ[1] > maxXYZ[1]?maxXYZ[1]:pointXYZ[1],
			pointXYZ[2] < minXYZ[2]?minXYZ[2]:pointXYZ[2] > maxXYZ[2]?maxXYZ[2]:pointXYZ[2]
		);
	}

	/**
	 * @returns min x,y,z vertex
	 */
	inline Vector3& getMin() {
		return min;
	}

	/**
	 * @returns max x,y,z vertex
	 */
	inline Vector3& getMax() {
		return max;
	}

	/**
	 * Returns bounding box vertices
	 * @returns vertices
	 */
	const vector<Vector3>& getVertices() const {
		return vertices;
	}

	/**
	 * @returns faces vertices indexes
	 */
	inline static const array<array<int32_t,3>,12>* getFacesVerticesIndexes() {
		return &facesVerticesIndexes;
	}

	/**
	 * @returns center
	 */
	inline const Vector3& getCenter() const {
		return center;
	}

	/**
	 * @returns half extension
	 */
	inline const Vector3& getDimensions() const {
		return dimensions;
	}

	/**
	 * Set up this bounding volume from given bounding volume
	 * @param original original bounding box
	 */
	void fromBoundingVolume(BoundingBox* original);

	/**
	 * Create bounding volume from given original(of same type) with applied transform matrix
	 * @param original original bounding box
	 * @param transformMatrix transform matrix
	 */
	void fromBoundingVolumeWithTransformMatrix(BoundingBox* original, const Matrix4x4& transformMatrix);

	/**
	 * Create bounding volume from given original(of same type) with applied transform
	 * @param original original bounding box
	 * @param transform transform
	 */
	inline void fromBoundingVolumeWithTransform(BoundingBox* original, const Transform& transform) {
		fromBoundingVolumeWithTransformMatrix(original, transform.getTransformMatrix());
	}

	/**
	 * Extend bounding box with given bounding box
	 * @param boundingBox bounding box
	 */
	inline void extend(BoundingBox* boundingBox) {
		for (auto i = 0; i < 3; i++) {
			if (boundingBox->getMin()[i] < min[i]) min[i] = boundingBox->getMin()[i];
			if (boundingBox->getMax()[i] > max[i]) max[i] = boundingBox->getMax()[i];
		}
		update();
	}

	/**
	 * Extend bounding box with given point
	 * @param point point
	 */
	inline void extend(const Vector3& point) {
		for (auto i = 0; i < 3; i++) {
			if (point[i] < min[i]) min[i] = point[i];
			if (point[i] > max[i]) max[i] = point[i];
		}
		update();
	}

	/**
	 * Updates this bounding box
	 */
	void update();

};
