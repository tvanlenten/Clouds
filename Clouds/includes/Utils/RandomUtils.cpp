#include "RandomUtils.h"
#include <time.h>


void Utils::SeedRandom()
{
	srand(time(NULL));
}

float Utils::Random(float minVal, float maxVal)
{
	return minVal + float(rand()) / (float(RAND_MAX) / (maxVal - minVal));
}

glm::vec2 Utils::Random(glm::vec2 minVal, glm::vec2 maxVal)
{
	return glm::vec2(Random(minVal.x, maxVal.x), Random(minVal.y, maxVal.y));
}

glm::vec3 Utils::Random(glm::vec3 minVal, glm::vec3 maxVal)
{
	return glm::vec3(Random(minVal.x, maxVal.x), Random(minVal.y, maxVal.y), Random(minVal.z, maxVal.z));
}

glm::vec4 Utils::Random(glm::vec4 minVal, glm::vec4 maxVal)
{
	return glm::vec4(Random(minVal.x, maxVal.x), Random(minVal.y, maxVal.y), Random(minVal.z, maxVal.z), Random(minVal.w, maxVal.w));
}
