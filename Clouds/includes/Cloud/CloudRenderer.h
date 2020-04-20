#pragma once
#include <memory>
#include <glm/glm.hpp>


class RenderTarget;
class Shader;
class Camera;
class Texture3D;


class CloudRenderer
{
public:
	CloudRenderer(glm::ivec2 cloudPassDims, std::shared_ptr<Texture3D> cloudVolume);
	~CloudRenderer();

	void Draw(std::shared_ptr<RenderTarget> target, std::shared_ptr<Camera> camera);

private:
	std::shared_ptr<Shader> _cloudShader;
	std::shared_ptr<Texture3D> _cloudVolume;
};