#include <tdme/engine/subsystems/postprocessing/PostProcessingShader.h>

#include <string>
#include <unordered_map>

#include <tdme/tdme.h>
#include <tdme/engine/subsystems/postprocessing/PostProcessingShaderDefaultImplementation.h>
#include <tdme/engine/subsystems/postprocessing/PostProcessingShaderDepthBlurImplementation.h>
#include <tdme/engine/subsystems/postprocessing/PostProcessingShaderDesaturationImplementation.h>
#include <tdme/engine/subsystems/postprocessing/PostProcessingShaderImplementation.h>
#include <tdme/engine/subsystems/postprocessing/PostProcessingShaderLightScatteringImplementation.h>
#include <tdme/engine/subsystems/postprocessing/PostProcessingShaderSSAOImplementation.h>
#include <tdme/engine/subsystems/postprocessing/PostProcessingShaderSSAOMapImplementation.h>
#include <tdme/engine/subsystems/postprocessing/PostProcessingShaderVignetteImplementation.h>
#include <tdme/engine/subsystems/renderer/RendererBackend.h>
#include <tdme/engine/Engine.h>

using std::string;
using std::unordered_map;

using tdme::engine::subsystems::postprocessing::PostProcessingShader;
using tdme::engine::subsystems::postprocessing::PostProcessingShaderDefaultImplementation;
using tdme::engine::subsystems::postprocessing::PostProcessingShaderDepthBlurImplementation;
using tdme::engine::subsystems::postprocessing::PostProcessingShaderDesaturationImplementation;
using tdme::engine::subsystems::postprocessing::PostProcessingShaderImplementation;
using tdme::engine::subsystems::postprocessing::PostProcessingShaderLightScatteringImplementation;
using tdme::engine::subsystems::postprocessing::PostProcessingShaderSSAOImplementation;
using tdme::engine::subsystems::postprocessing::PostProcessingShaderSSAOMapImplementation;
using tdme::engine::subsystems::postprocessing::PostProcessingShaderVignetteImplementation;
using tdme::engine::subsystems::renderer::RendererBackend;
using tdme::engine::Engine;

PostProcessingShader::PostProcessingShader(RendererBackend* rendererBackend)
{
	if (PostProcessingShaderDepthBlurImplementation::isSupported(rendererBackend) == true) shaders["depth_blur"] = new PostProcessingShaderDepthBlurImplementation(rendererBackend);
	if (PostProcessingShaderDefaultImplementation::isSupported(rendererBackend) == true) shaders["default"] = new PostProcessingShaderDefaultImplementation(rendererBackend);
	if (PostProcessingShaderDesaturationImplementation::isSupported(rendererBackend) == true) shaders["desaturation"] = new PostProcessingShaderDesaturationImplementation(rendererBackend);
	if (PostProcessingShaderLightScatteringImplementation::isSupported(rendererBackend) == true) shaders["light_scattering"] = new PostProcessingShaderLightScatteringImplementation(rendererBackend);
	if (PostProcessingShaderSSAOMapImplementation::isSupported(rendererBackend) == true) shaders["ssao_map"] = new PostProcessingShaderSSAOMapImplementation(rendererBackend);
	if (PostProcessingShaderSSAOImplementation::isSupported(rendererBackend) == true) shaders["ssao"] = new PostProcessingShaderSSAOImplementation(rendererBackend);
	if (PostProcessingShaderVignetteImplementation::isSupported(rendererBackend) == true) shaders["vignette"] = new PostProcessingShaderVignetteImplementation(rendererBackend);
	implementation = nullptr;
}

PostProcessingShader::~PostProcessingShader()
{
	for (const auto& [shaderId, shader]: shaders) {
		delete shader;
	}
}

bool PostProcessingShader::isInitialized()
{
	auto initialized = true;
	for (const auto& [shaderId, shader]: shaders) {
		initialized&= shader->isInitialized();
	}
	return initialized;
}

void PostProcessingShader::initialize()
{
	for (const auto& [shaderId, shader]: shaders) {
		shader->initialize();
	}
}

void PostProcessingShader::dispose()
{
	for (const auto& [shaderId, shader]: shaders) {
		shader->unloadTextures();
	}
}

void PostProcessingShader::useProgram()
{
	running = true;
}

void PostProcessingShader::unUseProgram()
{
	running = false;
	if (implementation != nullptr) {
		implementation->unUseProgram();;
	}
	implementation = nullptr;
}

bool PostProcessingShader::hasShader(const string& id) {
	return shaders.find(id) != shaders.end();
}

void PostProcessingShader::setShader(int contextIdx, const string& id) {
	if (running == false) return;

	auto currentImplementation = implementation;
	auto shaderIt = shaders.find(id);
	if (shaderIt == shaders.end()) {
		shaderIt = shaders.find("default");
	}
	implementation = shaderIt->second;

	if (currentImplementation != implementation) {
		if (currentImplementation != nullptr) currentImplementation->unUseProgram();
		implementation->useProgram(contextIdx);
	}
}

void PostProcessingShader::setBufferPixelWidth(int contextIdx, float pixelWidth) {
	if (implementation == nullptr) return;
	implementation->setBufferPixelWidth(contextIdx, pixelWidth);
}

void PostProcessingShader::setBufferPixelHeight(int contextIdx, float pixelHeight) {
	if (implementation == nullptr) return;
	implementation->setBufferPixelHeight(contextIdx, pixelHeight);
}

void PostProcessingShader::setShaderParameters(int contextIdx, Engine* engine) {
	if (implementation == nullptr) return;
	implementation->setShaderParameters(contextIdx, engine);
}

void PostProcessingShader::loadTextures(const string& pathName) {
	for (const auto& [shaderId, shader]: shaders) {
		shader->unloadTextures();
		shader->loadTextures(pathName);
	}
}
