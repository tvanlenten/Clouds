#pragma once
#include <glm/glm.hpp>

namespace Utils
{
	void SeedRandom();

	float Random(float minVal, float maxVal);

	glm::vec2 Random(glm::vec2 minVal, glm::vec2 maxVal);

	glm::vec3 Random(glm::vec3 minVal, glm::vec3 maxVal);

	glm::vec4 Random(glm::vec4 minVal, glm::vec4 maxVal);
}