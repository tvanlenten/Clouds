#pragma once
#include <memory>
#include <glm/glm.hpp>

class VertexArray;
class Shader;
class RenderTarget;
class TextureCubeMap;
class Camera;


class SkyboxRenderer
{
public:
	SkyboxRenderer(std::shared_ptr<TextureCubeMap> skybox);
	~SkyboxRenderer();
	
	void Gui();

	void Draw(std::shared_ptr<RenderTarget> target, std::shared_ptr<Camera> camera);

private:
	std::shared_ptr<VertexArray> _skyboxBox;
	std::shared_ptr<Shader> _skyboxShader;
	std::shared_ptr<TextureCubeMap> _skyboxTexture;
};