#include "CloudGenerator.h"

#include "../OpenGL/ComputeShader.h"
#include "../OpenGL/Texture3D.h"

CloudGenerator::CloudGenerator(glm::ivec3 dims)
{
	_cloudVolumeDims = dims;
	_groupDims = _cloudVolumeDims / 8;

	// just a single channel 32bit float texture for now
	_cloudVolume = std::make_shared<Texture3D>(nullptr, dims, GL_R32F, GL_RED, GL_FLOAT, false);
	_cloudVolume->bind();
	_cloudVolume->setFilter(GL_LINEAR);
	_cloudVolume->setWrapping(GL_REPEAT);

	_cloudGenerator = std::make_shared<ComputeShader>("shaders/cloudGenerator.comp", false);
}

CloudGenerator::~CloudGenerator()
{
}

std::shared_ptr<Texture3D> CloudGenerator::Generate()
{
	// all code relating to a shader should be between start and end
	_cloudGenerator->Start();

	// bind the volume to 0 so the compute shader can write to it
	_cloudVolume->bindTo(0);

	_cloudGenerator->Set("volumeDims", _cloudVolumeDims);

	// dispatch the compute shader using 1/8 the size of the texture
	// the compute shader is using work groups of size 8^3(512)
	// this may need to be reduced for different hardware!
	_cloudGenerator->Dispatch(_groupDims);
	_cloudGenerator->End();

	return _cloudVolume;
}