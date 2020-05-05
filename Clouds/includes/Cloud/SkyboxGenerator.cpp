#include "SkyboxGenerator.h"

#include "../OpenGL/TextureCubeMap.h"
#include "../OpenGL/ComputeShader.h"

SkyboxGenerator::SkyboxGenerator(glm::ivec2 cubemapFaceDims)
{
	_cubemapFaceDimensions = cubemapFaceDims;
	_groupDims = glm::ivec3(cubemapFaceDims / 16, 6);
	_cubemapTex = std::make_shared<TextureCubeMap>(_cubemapFaceDimensions, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, false);
	_skyboxGenShader = std::make_shared<ComputeShader>("shaders/skyboxGenerator.comp", false);
	_skyboxGenShader->Start();
	_skyboxGenShader->Set("texDims", _cubemapFaceDimensions);
	_skyboxGenShader->End();

	// some value that will never be used
	_lastSunDir = glm::vec3(-9999.0);
	_lastSunPower = -9999.0;
}

SkyboxGenerator::~SkyboxGenerator()
{
}

std::shared_ptr<TextureCubeMap> SkyboxGenerator::Generate(glm::vec3 sunDir, float sunPower)
{
	if (sunDir != _lastSunDir || sunPower != _lastSunPower)
	{
		_lastSunDir = sunDir;
		_lastSunPower = sunPower;

		_skyboxGenShader->Start();

		_cubemapTex->bindTo(0);
		_skyboxGenShader->Set("sunDir", sunDir);
		_skyboxGenShader->Set("sunPower", sunPower);

		_skyboxGenShader->Dispatch(_groupDims);
		_skyboxGenShader->End();
		glMemoryBarrier(GL_ALL_BARRIER_BITS); // wait until finished generating
	}

	return _cubemapTex;
}

std::shared_ptr<TextureCubeMap> SkyboxGenerator::GetCubemapTexture()
{
	return _cubemapTex;
}
