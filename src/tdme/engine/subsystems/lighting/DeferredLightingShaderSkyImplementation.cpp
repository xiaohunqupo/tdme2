#include <tdme/engine/subsystems/lighting/DeferredLightingShaderSkyImplementation.h>

#include <string>

#include <tdme/engine/subsystems/renderer/Renderer.h>
#include <tdme/engine/Engine.h>
#include <tdme/os/filesystem/FileSystem.h>
#include <tdme/os/filesystem/FileSystemInterface.h>

using std::string;
using std::to_string;

using tdme::engine::subsystems::lighting::LightingShaderBaseImplementation;
using tdme::engine::subsystems::lighting::DeferredLightingShaderSkyImplementation;
using tdme::engine::subsystems::renderer::Renderer;
using tdme::engine::Engine;
using tdme::os::filesystem::FileSystem;
using tdme::os::filesystem::FileSystemInterface;

bool DeferredLightingShaderSkyImplementation::isSupported(Renderer* renderer) {
	return renderer->isDeferredShadingAvailable() == true;
}

DeferredLightingShaderSkyImplementation::DeferredLightingShaderSkyImplementation(Renderer* renderer): LightingShaderBaseImplementation(renderer)
{
}

const string DeferredLightingShaderSkyImplementation::getId() {
	return "defer_sky";
}

void DeferredLightingShaderSkyImplementation::initialize()
{
	auto shaderVersion = renderer->getShaderVersion();

	// lighting
	//	fragment shader
	renderLightingFragmentShaderId = renderer->loadShader(
		renderer->SHADER_FRAGMENT_SHADER,
		"shader/" + shaderVersion + "/lighting/specular",
		"defer_fragmentshader.frag",
		"#define HAVE_SOLID_SHADING\n#define HAVE_BACK"
	);
	if (renderLightingFragmentShaderId == 0) return;

	//	vertex shader
	renderLightingVertexShaderId = renderer->loadShader(
		renderer->SHADER_VERTEX_SHADER,
		"shader/" + shaderVersion + "/lighting/specular",
		"render_vertexshader.vert",
		"#define HAVE_SOLID_SHADING\n#define HAVE_BACK"
	);
	if (renderLightingVertexShaderId == 0) return;

	// create, attach and link program
	programId = renderer->createProgram(renderer->PROGRAM_OBJECTS);
	renderer->attachShaderToProgram(programId, renderLightingVertexShaderId);
	renderer->attachShaderToProgram(programId, renderLightingFragmentShaderId);

	//
	LightingShaderBaseImplementation::initialize();
}

void DeferredLightingShaderSkyImplementation::registerShader() {
}


void DeferredLightingShaderSkyImplementation::updateShaderParameters(Renderer* renderer, void* context) {
}