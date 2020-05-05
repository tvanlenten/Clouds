#include "SceneRenderer.h"

#include <glm/gtx/transform.hpp>

#include "../OpenGL/RenderTarget.h"
#include "../OpenGL/VertexArray.h"
#include "../OpenGL/Shader.h"
#include "../OpenGL/MeshGenerator.h"
#include "../OpenGL/TextureCubeMap.h"
#include "../OpenGL/OpenGLState.h"
#include "../OpenGL/Camera.h"
#include "../OpenGL/Texture.h"

#include "../ImGui/imgui.h"

#include "../Utils/RandomUtils.h"
#include <iostream>


SceneRenderer::SceneRenderer()
{
	_phongShader = std::make_shared<Shader>("shaders/terrain.vert", "shaders/terrain.frag", nullptr, false);
	// Must bind shader before setting a uniform variable!
	_phongShader->Start();
	_phongShader->Set("skybox", 0);
	_phongShader->End();

	
	_terrainScale = glm::vec3(32.0, 4.0, 32.0);
	// create terrain
	int hWidth, hHeight;
	float* heightMapData = Texture::loadHeightMap("textures/heightmap2.png", &hWidth, &hHeight);
	if (heightMapData != nullptr)
	{
		_terrain = generateTerrain(heightMapData, glm::ivec2(hWidth, hHeight), _terrainScale);
		Texture::freeImage(heightMapData);
	}
	else
	{
		_terrain = generateBox(glm::vec3(0.0), glm::vec3(1.0));
	}
}

SceneRenderer::~SceneRenderer()
{
}

void SceneRenderer::Gui()
{

}

void SceneRenderer::Draw(std::shared_ptr<RenderTarget> target, std::shared_ptr<TextureCubeMap> skyboxTexture, std::shared_ptr<Camera> camera, glm::vec3 lightDir)
{
	CullFace();

	_phongShader->Start();

	auto model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-16.0, 2.0, -16.0));
	_phongShader->Set("model", model);
	_phongShader->Set("normalMatrix", GetNormalMatrix(model));
	_phongShader->Set("projView", camera->ProjectionViewMatrix());
	_phongShader->Set("lightDir", lightDir);
	_phongShader->Set("cameraPos", camera->Position);

	_terrain->draw();
	_phongShader->End();

	DisableCullFace();
}
