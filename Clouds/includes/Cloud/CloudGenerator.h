#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "../OpenGL/Buffer.h"

class ComputeShader;
class Texture3D;

class CloudGenerator
{
public:
	CloudGenerator(glm::ivec3 dims, glm::i32 numCellDivisions);
	~CloudGenerator();

	std::shared_ptr<Texture3D> Generate();

private:
	std::shared_ptr<ComputeShader> _cloudGenerator;
	std::shared_ptr<Texture3D> _cloudVolume;
	glm::ivec3 _cloudVolumeDims;
	glm::ivec3 _groupDims;
	glm::i32 _numCellDivisions;

	std::shared_ptr<Buffer> CreateWorleyNoisePointBuffer(glm::i32 numCellsPerAxis);
};