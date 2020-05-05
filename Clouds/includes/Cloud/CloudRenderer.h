#pragma once
#include <memory>
#include <glm/glm.hpp>


class RenderTarget;
class Shader;
class Camera;
class Texture3D;
class TextureCubeMap;


class CloudRenderer
{
public:
	CloudRenderer(glm::ivec2 cloudPassDims, std::shared_ptr<Texture3D> cloudVolume);
	~CloudRenderer();

	void Draw(std::shared_ptr<RenderTarget> target, std::shared_ptr<Camera> camera, std::shared_ptr<TextureCubeMap> skybox, glm::vec3 sunDir, float sunPower, float time);

	void Gui();

	void SetCloudVolume(std::shared_ptr<Texture3D> cloudVolume);

private:
	std::shared_ptr<Shader> _cloudShader;
	std::shared_ptr<Texture3D> _cloudVolume;
	std::shared_ptr<RenderTarget> _cloudTarget;

	// Cloud shape variables
	glm::f32 _xScale_DEBUG = 0.1f;
	glm::f32 _yScale_DEBUG = 0.1f;
	glm::f32 _zScale_DEBUG = 0.1f;
	glm::f32 _cloudDefinition_DEBUG = 0.43f;
	glm::f32 _cloudTrim_DEBUG = 0.85f;
	glm::f32 _cloudHeight_DEBUG = 8.0f;
	glm::f32 _cloudSlice_DEBUG = 2.5f;
	glm::f32 _cloudDensity_DEBUG = 1.0f;

	float _rayMarchstepSize;
};