#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

class VertexArray;
class Buffer;

/*
	attachments
	0 - position
	1 - uv
*/
std::shared_ptr<VertexArray> generateScreenQuad();

/*
	attachments
	0 - position
	1 - normal
	2 - uv
*/
std::shared_ptr<VertexArray> generateBox(glm::vec3 min, glm::vec3 max);

/*
	attachments
	0 - position
*/
std::shared_ptr<VertexArray> generateBoundingBox(glm::vec3 min, glm::vec3 max);

/*
	attachments
	0 - position
	1 - normal
*/
std::shared_ptr<VertexArray> generateQuad3D(glm::vec3 position, glm::vec2 dim);

/*
	attachments
	0 - position
	1 - normal
	2 - uv
	3 - minPos (instanced)
	4 - maxPos (instanced)
*/
std::shared_ptr<VertexArray> generateBoxs(glm::vec3 min, glm::vec3 max, glm::vec3* bbMin, glm::vec3* bbMax, unsigned int numInstances);

/*
	attachments
	0 - position
	1 - normal
	2 - uv
*/
//std::shared_ptr<VertexArray> generateNormalizeCubeSphere();

/*
	attachments
	0 - position
*/
std::shared_ptr<VertexArray> generateCircle2D(float radius, unsigned int numSlices, bool filled, float yAspect = 1.0f);

/*
	attachments
	0 - position
	1 - normal
*/
std::shared_ptr<VertexArray> generateCylinder(glm::vec3 position, float radius, float height, unsigned int numSlices);

/*
	attachments
	0 - position
	1 - normal
*/
std::shared_ptr<VertexArray> generateCone(glm::vec3 position, float radius, float height, unsigned int numSlices);

/*
	attachments
	0 - position
	1 - normal
*/
std::shared_ptr<VertexArray> generateArrow(float coneRadius, float cylinderRadius, float percentCone, float height, unsigned int numSlices);

/*
	creates new scaled array of verts
	DELETE AFTER USE!
*/
glm::vec2* scaleVerts2D(glm::vec2* verts, unsigned int numVerts, glm::vec2 scale, glm::vec2 offset);

/*
	creates new scaled array of verts
	DELETE AFTER USE!
*/
glm::vec3* scaleVerts3D(glm::vec3* verts, unsigned int numVerts, glm::vec3 scale, glm::vec3 offset);


void addTri(std::vector<glm::vec3>* positions, std::vector<glm::vec3>* normals, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 normal);

void addQuad(std::vector<glm::vec3>* positions, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* texCoords, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, glm::vec3 norm);




class BoundingBoxArray
{
public:
	BoundingBoxArray(int maxBoundingBoxes);
	~BoundingBoxArray();
	void update(glm::vec3* bbMin, glm::vec3* bbMax, unsigned int numInstances);
	std::shared_ptr<VertexArray> getVAO();
private:
	std::shared_ptr<Buffer> _minPos;
	std::shared_ptr<Buffer> _maxPos;
	std::shared_ptr<VertexArray> _vao;
};