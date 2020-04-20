#pragma once
#include <glm/glm.hpp>

namespace Utils
{
	double lerp(double x, double y, double mix);

	double bilinear(const double& tx, const double& ty,
		const float& c00, const float& c10, const float& c01, const float& c11);

	/*
		calculate gradient of float texture
		no linear interpolation
	*/
	glm::vec2 SimpleGrad(float* tex, glm::ivec2 texDims, glm::ivec2 coord);

	glm::vec3 HeightMapNormal(float* tex, glm::ivec2 texDims, glm::ivec2 coord, glm::vec3 scale);
}