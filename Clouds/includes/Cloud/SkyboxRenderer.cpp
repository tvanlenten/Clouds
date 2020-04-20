#include "SkyboxRenderer.h"

#include "../OpenGL/RenderTarget.h"
#include "../OpenGL/VertexArray.h"
#include "../OpenGL/Shader.h"
#include "../OpenGL/TextureCubeMap.h"
#include "../OpenGL/MeshGenerator.h"
#include "../OpenGL/OpenGLState.h"
#include "../ImGui/imgui.h"
#include "../OpenGL/Camera.h"

SkyboxRenderer::SkyboxRenderer(std::shared_ptr<TextureCubeMap> skybox)
{
	_skyboxTexture = skybox;

	_skyboxShader = std::make_shared<Shader>("shaders/skybox.vert", "shaders/skybox.frag", nullptr, false);
	_skyboxShader->Start();
	_skyboxShader->Set("skybox", 0);
	_skyboxShader->End();
	
	_skyboxBox = generateBox(glm::vec3(-1.0), glm::vec3(1.0));
}

SkyboxRenderer::~SkyboxRenderer()
{
	
}

void SkyboxRenderer::Gui()
{

}

void SkyboxRenderer::Draw(std::shared_ptr<RenderTarget> target, std::shared_ptr<Camera> camera)
{
	auto projMatrix = camera->ProjectionMatrix();
	auto viewMatrix = camera->ViewMatrix();

	DisableDepthTest();

	// skybox cube will not move with camera
	viewMatrix[3][0] = 0.0f;
	viewMatrix[3][1] = 0.0f;
	viewMatrix[3][2] = 0.0f;

	auto projView = projMatrix * viewMatrix;

	_skyboxShader->Start();

	_skyboxShader->Set("projView", projView);
	_skyboxTexture->use(0);

	_skyboxBox->draw();
	_skyboxShader->End();

	DepthTest();
}

