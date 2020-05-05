#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <vector>

class RenderTarget;
class VertexArray;
class Shader;
class TextureCubeMap;
class Camera;
class Texture2D;


class SceneRenderer
{
public:
	SceneRenderer();
	~SceneRenderer();

	void Gui();

	void Draw(std::shared_ptr<RenderTarget> target, std::shared_ptr<TextureCubeMap> skyboxTexture, std::shared_ptr<Camera> camera, glm::vec3 sunDir, float sunPower);

private:
	std::shared_ptr<Shader> _terrainShader;
	std::shared_ptr<VertexArray> _terrain;
	glm::vec3 _terrainScale;

	std::shared_ptr<Texture2D> _terrainTex0;
	std::shared_ptr<Texture2D> _terrainTex1;

	glm::vec3 _lightPos;
};