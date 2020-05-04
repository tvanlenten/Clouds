#include "LinearInterpolation.h"

double Utils::lerp(double x, double y, double mix)
{
	return x * (1.0 - mix) + y * mix;
}

double Utils::bilinear(
	const double& tx, const double& ty, 
	const float& c00, const float& c10, 
	const float& c01, const float& c11)
{
	return (1 - tx) * (1 - ty) * c00 +
		tx * (1 - ty) * c10 +
		(1 - tx) * ty * c01 +
		tx * ty * c11;
}

glm::vec2 Utils::SimpleGrad(float* tex, glm::ivec2 texDims, glm::ivec2 coord)
{
	glm::ivec2 x0 = glm::clamp(glm::ivec2(coord.x - 1, coord.y), glm::ivec2(0), texDims);
	glm::ivec2 x1 = glm::clamp(glm::ivec2(coord.x + 1, coord.y), glm::ivec2(0), texDims);
	glm::ivec2 y0 = glm::clamp(glm::ivec2(coord.x, coord.y - 1), glm::ivec2(0), texDims);
	glm::ivec2 y1 = glm::clamp(glm::ivec2(coord.x, coord.y + 1), glm::ivec2(0), texDims);

	float xGrad = tex[x1.x + (x1.y * texDims.x)] - tex[x0.x + (x0.y * texDims.x)];
	float yGrad = tex[y1.x + (y1.y * texDims.x)] - tex[y0.x + (y0.y * texDims.x)];

	return glm::normalize(glm::vec2(xGrad, yGrad));
}

glm::vec3 Utils::HeightMapNormal(float* tex, glm::ivec2 texDims, glm::ivec2 coord, glm::vec3 scale)
{
	glm::ivec2 xMinCoord = glm::clamp(glm::ivec2(coord.x - 1, coord.y), glm::ivec2(0), texDims);
	glm::ivec2 xMaxCoord = glm::clamp(glm::ivec2(coord.x + 1, coord.y), glm::ivec2(0), texDims);
	glm::ivec2 yMinCoord = glm::clamp(glm::ivec2(coord.x, coord.y - 1), glm::ivec2(0), texDims);
	glm::ivec2 yMaxCoord = glm::clamp(glm::ivec2(coord.x, coord.y + 1), glm::ivec2(0), texDims);

	float xMinY = tex[xMinCoord.x + (xMinCoord.y * texDims.x)];
	float xMaxY = tex[xMaxCoord.x + (xMaxCoord.y * texDims.x)];
	float yMinY = tex[yMinCoord.x + (yMinCoord.y * texDims.x)];
	float yMaxY = tex[yMaxCoord.x + (yMaxCoord.y * texDims.x)];

	glm::vec2 xMinXZ = glm::vec2(xMinCoord) / glm::vec2(texDims);
	glm::vec2 xMaxXZ = glm::vec2(xMaxCoord) / glm::vec2(texDims);
	glm::vec2 yMinXZ = glm::vec2(yMinCoord) / glm::vec2(texDims);
	glm::vec2 yMaxXZ = glm::vec2(yMaxCoord) / glm::vec2(texDims);

	glm::vec3 xMin = glm::vec3(xMinXZ.x, xMinY, xMinXZ.y) * scale;
	glm::vec3 xMax = glm::vec3(xMaxXZ.x, xMaxY, xMaxXZ.y) * scale;
	glm::vec3 yMin = glm::vec3(yMinXZ.x, yMinY, yMinXZ.y) * scale;
	glm::vec3 yMax = glm::vec3(yMaxXZ.x, yMaxY, yMaxXZ.y) * scale;

	glm::vec3 norm = glm::cross(xMax - xMin, yMax - yMin);

	return glm::normalize(glm::vec3(norm.x, -norm.y, norm.z));
}
