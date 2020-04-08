#pragma once
#include <memory>
#include <glm/glm.hpp>

class VertexArray;
class Shader;
class RenderTarget;
class TextureCubeMap;


class SkyboxRenderer
{
public:
	SkyboxRenderer(std::shared_ptr<TextureCubeMap> skybox);
	~SkyboxRenderer();

	std::shared_ptr<RenderTarget> Draw(std::shared_ptr<RenderTarget> target, glm::mat4 projMatrix, glm::mat4 viewMatrix);

private:
	std::shared_ptr<VertexArray> _skyboxBox;
	std::shared_ptr<Shader> _skyboxShader;
	std::shared_ptr<TextureCubeMap> _skyboxTexture;
};