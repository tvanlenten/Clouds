#include "CloudGenerator.h"

CloudGenerator::CloudGenerator(glm::ivec3 dims)
{
}

CloudGenerator::~CloudGenerator()
{
}

std::shared_ptr<Texture3D> CloudGenerator::Generate()
{
	return std::shared_ptr<Texture3D>();
}

std::shared_ptr<Texture3D> CloudGenerator::GetCloudVolume()
{
	return std::shared_ptr<Texture3D>();
}
