#include <iostream>

#include "CloudGenerator.h"
#include "../Utils/RandomUtils.h"

#include "../OpenGL/ComputeShader.h"
#include "../OpenGL/Texture3D.h"

CloudGenerator::CloudGenerator(glm::ivec3 dims, int numCellDivisions)
{
	_cloudVolumeDims = dims;
	_groupDims = _cloudVolumeDims / 8;
	_numCellDivisions = numCellDivisions;

	// just a single channel 32bit float texture for now
	_cloudVolume = std::make_shared<Texture3D>(nullptr, dims, GL_RGBA32F, GL_RGBA, GL_FLOAT, false);
	_cloudVolume->bind();
	_cloudVolume->setFilter(GL_LINEAR);
	_cloudVolume->setWrapping(GL_REPEAT);

	Utils::SeedRandom();

	_cloudGenerator = std::make_shared<ComputeShader>("shaders/cloudGenerator.comp", false);
}

CloudGenerator::~CloudGenerator()
{
}

std::shared_ptr<Buffer> CloudGenerator::CreateWorleyNoisePointBuffer(int numCellsPerAxis)
{
	auto numTotalCells = numCellsPerAxis * numCellsPerAxis * numCellsPerAxis;
	// USE VEC4 BECAUSE COMPUTE BUFFERS DON'T WORK WITH VEC3
	auto points = new glm::vec4[numTotalCells];
	auto cellSize = glm::float32(1.0f) / numCellsPerAxis;

	for (int x = 0; x < numCellsPerAxis; x++) {
		for (int y = 0; y < numCellsPerAxis; y++) {
			for (int z = 0; z < numCellsPerAxis; z++) {
				auto randomOffset = Utils::Random(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
				auto position = (glm::vec3(x, y, z) + randomOffset) * cellSize;
				auto index = x + numCellsPerAxis * (y + z * numCellsPerAxis);
				points[index] = glm::vec4(position, 0.0);
			}
		}
	}

	// Create the buffer with the random point data
	auto pointBuffer = std::make_shared<Buffer>(GL_SHADER_STORAGE_BUFFER, numTotalCells * sizeof(glm::vec4), &points[0], GL_STATIC_READ);
	return pointBuffer;
}


void CloudGenerator::GenerateInternal(int numCellDivisions, int channel)
{
	// Generate random point buffer for worley noise
	auto worleyNoisePointBuffer = CreateWorleyNoisePointBuffer(numCellDivisions);

	// all code relating to a shader should be between start and end
	_cloudGenerator->Start();

	// bind the volume to 0 so the compute shader can write to it
	_cloudVolume->bindTo(0);

	// bind the random point buffer to 2 so the compute shader can access it
	worleyNoisePointBuffer->bindTo(2);

	_cloudGenerator->Set("volumeDims", _cloudVolumeDims);
	_cloudGenerator->Set("numCellsPerAxis", numCellDivisions);
	_cloudGenerator->Set("invert", true);
	_cloudGenerator->Set("channel", channel);

	// dispatch the compute shader using 1/8 the size of the texture
	// the compute shader is using work groups of size 8^3(512)
	// this may need to be reduced for different hardware!
	_cloudGenerator->Dispatch(_groupDims);
	_cloudGenerator->End();
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}


std::shared_ptr<Texture3D> CloudGenerator::Generate()
{
	GenerateInternal(4, 0);
	GenerateInternal(8, 1);
	GenerateInternal(16, 2);
	GenerateInternal(32, 3);

	return _cloudVolume;
}