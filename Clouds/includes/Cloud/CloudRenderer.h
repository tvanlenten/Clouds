#pragma once
#include <memory>
#include <glm/glm.hpp>


class RenderTarget;
class VertexArray;
class Shader;


class CloudRenderer
{
public:
	CloudRenderer();
	~CloudRenderer();

	std::shared_ptr<RenderTarget> Draw(std::shared_ptr<RenderTarget> target, glm::mat4 invProjViewMatrix);

private:
	std::shared_ptr<VertexArray> _screenQuad;
	std::shared_ptr<Shader> _cloudShader;
};