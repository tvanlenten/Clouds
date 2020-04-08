#pragma once
#include <memory>
#include <glm/glm.hpp>

class Shader;
class Texture3D;

class CloudGenerator
{
public:
	CloudGenerator(glm::ivec3 dims);
	~CloudGenerator();

	std::shared_ptr<Texture3D> Generate(/*some gen prms?*/);
	std::shared_ptr<Texture3D> GetCloudVolume();

private:
	std::shared_ptr<Shader> _cloudGenerator;
	std::shared_ptr<Texture3D> _cloudVolume;
	glm::vec3 _cloudVolumeDims;

};