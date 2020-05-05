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
#include "../OpenGL/Texture2D.h"

#include "../ImGui/imgui.h"

#include "../Utils/RandomUtils.h"
#include <iostream>


SceneRenderer::SceneRenderer()
{
	_terrainShader = std::make_shared<Shader>("shaders/terrain.vert", "shaders/terrain.frag", nullptr, false);
	// Must bind shader before setting a uniform variable!
	_terrainShader->Start();
	_terrainShader->Set("tex0", 0);
	_terrainShader->Set("tex1", 1);
	_terrainShader->End();

	
	_terrainScale = glm::vec3(32.0, 4.0, 32.0);
	// create terrain
	int hWidth, hHeight;
	float* heightMapData = Texture::loadHeightMap("textures/heightMapLow.png", &hWidth, &hHeight);
	if (heightMapData != nullptr)
	{
		_terrain = generateTerrain(heightMapData, glm::ivec2(hWidth, hHeight), _terrainScale);
		Texture::freeImage(heightMapData);
	}
	else
	{
		_terrain = generateBox(glm::vec3(0.0), glm::vec3(1.0));
	}

	_terrainTex0 = std::make_shared<Texture2D>("textures/grass.jpg");
	_terrainTex0->bind();
	_terrainTex0->setWrapping(GL_REPEAT);
	_terrainTex1 = std::make_shared<Texture2D>("textures/rock.png");
	_terrainTex1->bind();
	_terrainTex1->setWrapping(GL_REPEAT);
}

SceneRenderer::~SceneRenderer()
{
}

void SceneRenderer::Gui()
{

}

void SceneRenderer::Draw(std::shared_ptr<RenderTarget> target, std::shared_ptr<TextureCubeMap> skyboxTexture, std::shared_ptr<Camera> camera, glm::vec3 sunDir, float sunPower)
{
	CullFace();

	_terrainShader->Start();

	auto model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-16.0, 2.0, -16.0));
	_terrainShader->Set("model", model);
	_terrainShader->Set("normalMatrix", GetNormalMatrix(model));
	_terrainShader->Set("projView", camera->ProjectionViewMatrix());
	_terrainShader->Set("sunDir", sunDir);
	_terrainShader->Set("sunPower", sunPower);
	_terrainShader->Set("cameraPos", camera->Position);

	_terrainTex0->use(0);
	_terrainTex1->use(1);

	_terrain->draw();
	_terrainShader->End();

	DisableCullFace();
}
