#pragma once
#include <memory>
#include <glm/glm.hpp>

class ComputeShader;
class TextureCubeMap;

class SkyboxGenerator
{
public:
	SkyboxGenerator(glm::ivec2 cubemapFaceDims);
	~SkyboxGenerator();
	
	/*
		Generates the cubemap texture
	*/
	std::shared_ptr<TextureCubeMap> Generate(glm::vec3 sunPosition);

	/*
		Get the skybox cubemap texture
	*/
	std::shared_ptr<TextureCubeMap> GetCubemapTexture();

private:
	std::shared_ptr<ComputeShader> _skyboxGenShader;
	std::shared_ptr<TextureCubeMap> _cubemapTex;
	
	glm::ivec2 _cubemapFaceDimensions;
	glm::ivec3 _groupDims;
};