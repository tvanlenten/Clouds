#include "SceneRenderer.h"

#include <glm/gtx/transform.hpp>

#include "../OpenGL/RenderTarget.h"
#include "../OpenGL/VertexArray.h"
#include "../OpenGL/Shader.h"
#include "../OpenGL/MeshGenerator.h"
#include "../OpenGL/TextureCubeMap.h"
#include "../OpenGL/OpenGLState.h"

#include "../ImGui/imgui.h"

#include "../Utils/RandomUtils.h"


SceneRenderer::SceneRenderer()
{
	_phongShader = std::make_shared<Shader>("shaders/phong.vert", "shaders/phong.frag", nullptr, false);
	// Must bind shader before setting a uniform variable!
	_phongShader->Start();
	_phongShader->Set("skybox", 0);
	_phongShader->End();

	_testObject = generateBox(glm::vec3(-1.0), glm::vec3(1.0));//generateArrow(0.04, 0.01, 0.2, 0.5, 16);
	_lightPos = glm::vec3(-1.0, 1.3, 0.0);

	_objectRot = std::vector<glm::vec3>();
	_objectPos = std::vector<glm::vec3>();

	Utils::SeedRandom();

	for (int i = 0; i < 32; ++i)
	{
		_objectPos.push_back(Utils::Random(glm::vec3(-10.0), glm::vec3(10.0)));
		_objectRot.push_back(Utils::Random(glm::vec3(-1.0), glm::vec3(1.0)));
		_objectScale.push_back(Utils::Random(0.1,2.0));
		_objectCol.push_back(glm::vec4(Utils::Random(glm::vec3(0.0), glm::vec3(1.0)), 1.0));
	}
}

SceneRenderer::~SceneRenderer()
{
}

void SceneRenderer::Gui()
{
	ImGui::DragFloat3("LightPos", &_lightPos[0], 0.1);
}

void SceneRenderer::Update()
{
	for (int i = 0; i < _objectPos.size(); ++i)
	{
		
	}
}

void SceneRenderer::Draw(std::shared_ptr<RenderTarget> target, std::shared_ptr<TextureCubeMap> skyboxTexture, glm::mat4 projectionViewMatrix, glm::vec3 cameraPosition)
{
	CullFace();

	_phongShader->Start();

	_phongShader->Set("projView", projectionViewMatrix);
	_phongShader->Set("lightPos", _lightPos);
	_phongShader->Set("cameraPos", cameraPosition);

	for (int i = 0; i < _objectPos.size(); ++i)
	{
		auto model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(_objectScale[i]));
		model = glm::rotate(model, 1.0f, _objectRot[i]);
		model = glm::translate(model, _objectPos[i]);

		_phongShader->Set("model", model);
		_phongShader->Set("normalMatrix", GetNormalMatrix(model));
		_phongShader->Set("color", _objectCol[i]);

		skyboxTexture->use(0);
		_testObject->draw();
	}

	_phongShader->End();


	DisableCullFace();
}
