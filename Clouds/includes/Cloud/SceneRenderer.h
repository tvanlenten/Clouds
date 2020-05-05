#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <vector>

class RenderTarget;
class VertexArray;
class Shader;
class TextureCubeMap;
class Camera;


class SceneRenderer
{
public:
	SceneRenderer();
	~SceneRenderer();

	void Gui();

	void Update();

	void Draw(std::shared_ptr<RenderTarget> target, std::shared_ptr<TextureCubeMap> skyboxTexture, std::shared_ptr<Camera> camera);

private:
	std::shared_ptr<Shader> _phongShader;
	std::shared_ptr<VertexArray> _terrain;
	glm::vec3 _terrainScale;

	glm::vec3 _lightPos;
};