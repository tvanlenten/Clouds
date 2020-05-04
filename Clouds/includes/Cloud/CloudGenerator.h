#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "../OpenGL/Buffer.h"

class ComputeShader;
class Texture3D;

class CloudGenerator
{
public:
	CloudGenerator(glm::ivec3 dims, int numCellDivisions);
	~CloudGenerator();

	std::shared_ptr<Texture3D> Generate();

private:
	std::shared_ptr<ComputeShader> _cloudGenerator;
	std::shared_ptr<Texture3D> _cloudVolume;
	glm::ivec3 _cloudVolumeDims;
	glm::ivec3 _groupDims;
	int _numCellDivisions;

	void GenerateInternal(int numCellDivisions, int channel);

	std::shared_ptr<Buffer> CreateWorleyNoisePointBuffer(int numCellsPerAxis);
};