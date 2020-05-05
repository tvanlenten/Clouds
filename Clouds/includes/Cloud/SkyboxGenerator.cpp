#include "SkyboxGenerator.h"

#include "../OpenGL/TextureCubeMap.h"
#include "../OpenGL/ComputeShader.h"
#include "../OpenGL/TimeQuery.h"
#include "../ImGui/imgui.h"

#include <iostream>

SkyboxGenerator::SkyboxGenerator(glm::ivec2 cubemapFaceDims)
{
	_cubemapFaceDimensions = cubemapFaceDims;
	_groupDims = glm::ivec3(cubemapFaceDims / 16, 6);
	_cubemapTex = std::make_shared<TextureCubeMap>(_cubemapFaceDimensions, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, false);
	_skyboxGenShader = std::make_shared<ComputeShader>("shaders/skyboxGenerator.comp", false);
	_skyboxGenShader->Start();
	_skyboxGenShader->Set("texDims", _cubemapFaceDimensions);
	_skyboxGenShader->Set("starTex", 1);
	_skyboxGenShader->End();

	// some value that will never be used
	_lastSunDir = glm::vec3(-9999.0);
	_lastSunPower = -9999.0;

	std::string faces[] = {
		"textures/starCubemap/front.png", "textures/starCubemap/back.png",
		"textures/starCubemap/top.png", "textures/starCubemap/bottom.png",
		"textures/starCubemap/left.png", "textures/starCubemap/right.png" };
	_starsTex = std::make_shared<TextureCubeMap>(faces, false, true);

	_timer = std::make_shared<TimeQuery>();
}

SkyboxGenerator::~SkyboxGenerator()
{
}

std::shared_ptr<TextureCubeMap> SkyboxGenerator::Generate(glm::vec3 sunDir, float sunPower, bool debug)
{
	if (sunDir != _lastSunDir || sunPower != _lastSunPower)
	{
		if (debug) _timer->start();

		_lastSunDir = sunDir;
		_lastSunPower = sunPower;

		_skyboxGenShader->Start();

		_cubemapTex->bindTo(0);
		_starsTex->use(1);
		_skyboxGenShader->Set("sunDir", sunDir);
		_skyboxGenShader->Set("sunPower", sunPower);

		_skyboxGenShader->Dispatch(_groupDims);
		_skyboxGenShader->End();
		glMemoryBarrier(GL_ALL_BARRIER_BITS); // wait until finished generating

		if (debug)
		{
			_timer->stop();
			std::cout << "skybox generated in " << _timer->getDeltaTime() << "ms" << std::endl;
		}
	}

	return _cubemapTex;
}

std::shared_ptr<TextureCubeMap> SkyboxGenerator::GetCubemapTexture()
{
	return _cubemapTex;
}
