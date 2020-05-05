#pragma once
#include <memory>
#include <glm/glm.hpp>

class ComputeShader;
class TextureCubeMap;
class TimeQuery;

class SkyboxGenerator
{
public:
	SkyboxGenerator(glm::ivec2 cubemapFaceDims);
	~SkyboxGenerator();
	
	/*
		Generates the cubemap texture
	*/
	std::shared_ptr<TextureCubeMap> Generate(glm::vec3 sunDir, float sunPower, bool debug = false);

	/*
		Get the skybox cubemap texture
	*/
	std::shared_ptr<TextureCubeMap> GetCubemapTexture();

private:
	std::shared_ptr<ComputeShader> _skyboxGenShader;
	std::shared_ptr<TextureCubeMap> _cubemapTex;
	
	glm::ivec2 _cubemapFaceDimensions;
	glm::ivec3 _groupDims;

	std::shared_ptr<TimeQuery> _timer;

	glm::vec3 _lastSunDir;
	float _lastSunPower;
};