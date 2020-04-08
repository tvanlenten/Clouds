#include "SkyboxGenerator.h"

#include "../OpenGL/TextureCubeMap.h"
#include "../OpenGL/Shader.h"

SkyboxGenerator::SkyboxGenerator(glm::ivec2 cubemapFaceDims)
{
	_cubemapFaceDimensions = cubemapFaceDims;
}

SkyboxGenerator::~SkyboxGenerator()
{
}

std::shared_ptr<TextureCubeMap> SkyboxGenerator::Generate()
{
	return std::shared_ptr<TextureCubeMap>();
}

std::shared_ptr<TextureCubeMap> SkyboxGenerator::GetCubemapTexture()
{
	return std::shared_ptr<TextureCubeMap>();
}
