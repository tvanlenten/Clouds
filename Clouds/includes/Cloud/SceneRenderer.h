#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <vector>

class RenderTarget;
class VertexArray;
class Shader;
class TextureCubeMap;


class SceneRenderer
{
public:
	SceneRenderer();
	~SceneRenderer();

	void Gui();

	void Update();

	void Draw(std::shared_ptr<RenderTarget> target, std::shared_ptr<TextureCubeMap> skyboxTexture, glm::mat4 projectionViewMatrix, glm::vec3 cameraPosition);

private:
	std::shared_ptr<Shader> _phongShader;
	std::shared_ptr<VertexArray> _testObject;

	std::vector<glm::vec3> _objectRot;
	std::vector<glm::vec3> _objectPos;
	std::vector<float> _objectScale;
	std::vector<glm::vec4> _objectCol;

	glm::vec3 _lightPos;

};