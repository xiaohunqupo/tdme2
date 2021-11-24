#include <tdme/engine/subsystems/lighting/DeferredLightingShaderTreeImplementation.h>

#include <string>

#include <tdme/engine/subsystems/renderer/Renderer.h>
#include <tdme/engine/EntityShaderParameters.h>
#include <tdme/engine/ShaderParameter.h>
#include <tdme/os/filesystem/FileSystem.h>
#include <tdme/os/filesystem/FileSystemInterface.h>

using std::string;
using std::to_string;

using tdme::engine::subsystems::lighting::LightingShaderBaseImplementation;
using tdme::engine::subsystems::lighting::DeferredLightingShaderTreeImplementation;
using tdme::engine::subsystems::renderer::Renderer;
using tdme::engine::EntityShaderParameters;
using tdme::engine::ShaderParameter;
using tdme::os::filesystem::FileSystem;
using tdme::os::filesystem::FileSystemInterface;

bool DeferredLightingShaderTreeImplementation::isSupported(Renderer* renderer) {
	return renderer->isDeferredShadingAvailable() == true;
}

DeferredLightingShaderTreeImplementation::DeferredLightingShaderTreeImplementation(Renderer* renderer): LightingShaderBaseImplementation(renderer)
{
}

const string DeferredLightingShaderTreeImplementation::getId() {
	return "defer_tree";
}

void DeferredLightingShaderTreeImplementation::initialize()
{
	auto shaderVersion = renderer->getShaderVersion();

	// lighting
	//	fragment shader
	renderLightingFragmentShaderId = renderer->loadShader(
		renderer->SHADER_FRAGMENT_SHADER,
		"shader/" + shaderVersion + "/lighting/specular",
		"defer_fragmentshader.frag",
		"#define HAVE_DEPTH_FOG"
	);
	if (renderLightingFragmentShaderId == 0) return;

	//	vertex shader
	renderLightingVertexShaderId = renderer->loadShader(
		renderer->SHADER_VERTEX_SHADER,
		"shader/" + shaderVersion + "/lighting/specular",
		"render_vertexshader.vert",
		"#define HAVE_TREE\n#define HAVE_DEPTH_FOG",
		FileSystem::getInstance()->getContentAsString(
			"shader/" + shaderVersion + "/functions",
			"create_rotation_matrix.inc.glsl"
		) +
		"\n\n" +
		FileSystem::getInstance()->getContentAsString(
			"shader/" + shaderVersion + "/functions",
			"create_translation_matrix.inc.glsl"
		) +
		"\n\n" +
		FileSystem::getInstance()->getContentAsString(
			"shader/" + shaderVersion + "/functions",
			"create_tree_transform_matrix.inc.glsl"
		)
	);
	if (renderLightingVertexShaderId == 0) return;

	// create, attach and link program
	programId = renderer->createProgram(renderer->PROGRAM_OBJECTS);
	renderer->attachShaderToProgram(programId, renderLightingVertexShaderId);
	renderer->attachShaderToProgram(programId, renderLightingFragmentShaderId);

	//
	LightingShaderBaseImplementation::initialize();

	//
	if (initialized == false) return;

	// uniforms
	uniformSpeed = renderer->getProgramUniformLocation(programId, "speed");
}

void DeferredLightingShaderTreeImplementation::registerShader() {
}

void DeferredLightingShaderTreeImplementation::updateShaderParameters(Renderer* renderer, void* context) {
	auto& shaderParameters = renderer->getShaderParameters(context);
	if (uniformSpeed != -1) renderer->setProgramUniformFloat(context, uniformSpeed, shaderParameters.getShaderParameter("speed").getFloatValue());
}