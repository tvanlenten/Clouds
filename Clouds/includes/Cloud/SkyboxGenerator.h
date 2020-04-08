#pragma once
#include <memory>
#include <glm/glm.hpp>

class Shader;
class TextureCubeMap;

class SkyboxGenerator
{
public:
	SkyboxGenerator(glm::ivec2 cubemapFaceDims);
	~SkyboxGenerator();
	
	/*
		Generates the cubemap texture
	*/
	std::shared_ptr<TextureCubeMap> Generate();

	/*
		Get the skybox cubemap texture
	*/
	std::shared_ptr<TextureCubeMap> GetCubemapTexture();

private:
	std::shared_ptr<Shader> _skyboxGenShader;
	std::shared_ptr<TextureCubeMap> _cubemapTex;
	
	glm::ivec2 _cubemapFaceDimensions;
};