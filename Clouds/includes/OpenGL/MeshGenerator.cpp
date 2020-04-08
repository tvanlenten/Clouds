#include "MeshGenerator.h"
#include "Buffer.h"
#include "VertexArray.h"
#include <iostream>
#include <string>

#define _PI_ 3.14159265359

glm::vec2 unitQuadVerts[]
{
	glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f),
	glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f)
};

glm::vec2 normalizedQuadVerts[]
{
	glm::vec2(-1.0f, -1.0f), glm::vec2(1.0f, -1.0f), glm::vec2(1.0f, 1.0f),
	glm::vec2(-1.0f, -1.0f), glm::vec2(1.0f, 1.0f), glm::vec2(-1.0f, 1.0f)
};

std::shared_ptr<VertexArray> generateScreenQuad()
{
	std::shared_ptr<VertexArray> vao = std::make_shared<VertexArray>();
	vao->bind();
	vao->setVertexSize(6);

	std::shared_ptr<Buffer> posData = std::make_shared<Buffer>(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec2), normalizedQuadVerts);
	vao->attach(posData, AttachType::VEC2);

	std::shared_ptr<Buffer> uvData = std::make_shared<Buffer>(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec2), unitQuadVerts);
	vao->attach(uvData, AttachType::VEC2);

	vao->unbind();
	return vao;
}

std::shared_ptr<VertexArray> generateBox(glm::vec3 min, glm::vec3 max)
{
	glm::vec3 scale = max - min;
	glm::vec3 offset = min;

	std::vector<glm::vec3> posVerts = std::vector<glm::vec3>();
	std::vector<glm::vec3> normVerts = std::vector<glm::vec3>();
	std::vector<glm::vec2> uvVerts = std::vector<glm::vec2>();
	addQuad(&posVerts, &normVerts, &uvVerts, glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, min.y, min.z), glm::vec3(min.x, max.y, min.z), glm::vec3(max.x, max.y, min.z), glm::vec3(0.0, 0.0, -1.0));
	addQuad(&posVerts, &normVerts, &uvVerts, glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, min.y, max.z), glm::vec3(max.x, max.y, min.z), glm::vec3(max.x, max.y, max.z), glm::vec3(1.0, 0.0, 0.0));
	addQuad(&posVerts, &normVerts, &uvVerts, glm::vec3(max.x, min.y, max.z), glm::vec3(min.x, min.y, max.z), glm::vec3(max.x, max.y, max.z), glm::vec3(min.x, max.y, max.z), glm::vec3(0.0, 0.0, 1.0));
	addQuad(&posVerts, &normVerts, &uvVerts, glm::vec3(min.x, min.y, max.z), glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, max.y, max.z), glm::vec3(min.x, max.y, min.z), glm::vec3(-1.0, 0.0, 0.0));
	addQuad(&posVerts, &normVerts, &uvVerts, glm::vec3(min.x, max.y, min.z), glm::vec3(max.x, max.y, min.z), glm::vec3(min.x, max.y, max.z), glm::vec3(max.x, max.y, max.z), glm::vec3(0.0, 1.0, 0.0));
	addQuad(&posVerts, &normVerts, &uvVerts, glm::vec3(min.x, min.y, max.z), glm::vec3(max.x, min.y, max.z), glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, min.y, min.z), glm::vec3(0.0, -1.0, 0.0));

	std::shared_ptr<VertexArray> vao = std::make_shared<VertexArray>();
	vao->bind();
	vao->setVertexSize(36);

	std::shared_ptr<Buffer> posData = std::make_shared<Buffer>(GL_ARRAY_BUFFER, 36 * sizeof(glm::vec3), posVerts.data());
	vao->attach(posData, AttachType::VEC3);

	std::shared_ptr<Buffer> normData = std::make_shared<Buffer>(GL_ARRAY_BUFFER, 36 * sizeof(glm::vec3), normVerts.data());
	vao->attach(normData, AttachType::VEC3);

	std::shared_ptr<Buffer> uvData = std::make_shared<Buffer>(GL_ARRAY_BUFFER, 36 * sizeof(glm::vec2), uvVerts.data());
	vao->attach(uvData, AttachType::VEC2);

	vao->unbind();


	return vao;
}

std::shared_ptr<VertexArray> generateBoundingBox(glm::vec3 min, glm::vec3 max)
{
	glm::vec3 a = min;
	glm::vec3 b = max;
	GLfloat posVerts[] =
	{
		a.x, a.y, a.z,
		b.x, a.y, a.z,
		a.x, a.y, a.z,
		a.x, b.y, a.z,
		a.x, b.y, a.z,
		b.x, b.y, a.z,
		b.x, a.y, a.z,
		b.x, b.y, a.z,

		a.x, a.y, b.z,
		b.x, a.y, b.z,
		a.x, a.y, b.z,
		a.x, b.y, b.z,
		a.x, b.y, b.z,
		b.x, b.y, b.z,
		b.x, a.y, b.z,
		b.x, b.y, b.z,

		a.x, a.y, a.z,
		a.x, a.y, b.z,
		b.x, a.y, a.z,
		b.x, a.y, b.z,
		a.x, b.y, a.z,
		a.x, b.y, b.z,
		b.x, b.y, a.z,
		b.x, b.y, b.z,
	};

	std::shared_ptr<VertexArray> vao = std::make_shared<VertexArray>();
	vao->bind();
	vao->setPrimativeType(GL_LINES);
	vao->setVertexSize(24);
	std::shared_ptr<Buffer> posData = std::make_shared<Buffer>(GL_ARRAY_BUFFER, 24 * sizeof(glm::vec3), posVerts);
	vao->attach(posData, AttachType::VEC3);
	vao->unbind();

	return vao;
}

std::shared_ptr<VertexArray> generateQuad3D(glm::vec3 position, glm::vec2 dim)
{
	glm::vec3 posVerts[]
	{
		position + glm::vec3(0.0, 0.0, 0.0),
		position + glm::vec3(0.0, 0.0, dim.y),
		position + glm::vec3(dim.x, 0.0, 0.0),
		position + glm::vec3(dim.x, 0.0, dim.y)
	};
	glm::vec3 normalVerts[]
	{
		glm::vec3(0.0,1.0,0.0),
		glm::vec3(0.0,1.0,0.0),
		glm::vec3(0.0,1.0,0.0),
		glm::vec3(0.0,1.0,0.0)
	};

	std::shared_ptr<VertexArray> vao = std::make_shared<VertexArray>();
	vao->bind();
	vao->setVertexSize(4);
	vao->setPrimativeType(GL_TRIANGLE_STRIP);
	std::shared_ptr<Buffer> posData = std::make_shared<Buffer>(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), posVerts);
	vao->attach(posData, AttachType::VEC3);
	std::shared_ptr<Buffer> normalData = std::make_shared<Buffer>(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), normalVerts);
	vao->attach(normalData, AttachType::VEC3);
	vao->unbind();

	return vao;
}

std::shared_ptr<VertexArray> generateBoxs(glm::vec3 min, glm::vec3 max, glm::vec3* bbMin, glm::vec3* bbMax, unsigned int numInstances)
{
	glm::vec3 scale = max - min;
	glm::vec3 offset = min;

	std::vector<glm::vec3> posVerts = std::vector<glm::vec3>();
	std::vector<glm::vec3> normVerts = std::vector<glm::vec3>();
	std::vector<glm::vec2> uvVerts = std::vector<glm::vec2>();
	addQuad(&posVerts, &normVerts, &uvVerts, glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, min.y, min.z), glm::vec3(min.x, max.y, min.z), glm::vec3(max.x, max.y, min.z), glm::vec3(0.0, 0.0, -1.0));
	addQuad(&posVerts, &normVerts, &uvVerts, glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, min.y, max.z), glm::vec3(max.x, max.y, min.z), glm::vec3(max.x, max.y, max.z), glm::vec3(1.0, 0.0, 0.0));
	addQuad(&posVerts, &normVerts, &uvVerts, glm::vec3(max.x, min.y, max.z), glm::vec3(min.x, min.y, max.z), glm::vec3(max.x, max.y, max.z), glm::vec3(min.x, max.y, max.z), glm::vec3(0.0, 0.0, 1.0));
	addQuad(&posVerts, &normVerts, &uvVerts, glm::vec3(min.x, min.y, max.z), glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, max.y, max.z), glm::vec3(min.x, max.y, min.z), glm::vec3(-1.0, 0.0, 0.0));
	addQuad(&posVerts, &normVerts, &uvVerts, glm::vec3(min.x, max.y, min.z), glm::vec3(max.x, max.y, min.z), glm::vec3(min.x, max.y, max.z), glm::vec3(max.x, max.y, max.z), glm::vec3(0.0, 1.0, 0.0));
	addQuad(&posVerts, &normVerts, &uvVerts, glm::vec3(min.x, min.y, max.z), glm::vec3(max.x, min.y, max.z), glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, min.y, min.z), glm::vec3(0.0, -1.0, 0.0));

	std::shared_ptr<VertexArray> vao = std::make_shared<VertexArray>();
	vao->bind();
	vao->setVertexSize(36);

	std::shared_ptr<Buffer> posData = std::make_shared<Buffer>(GL_ARRAY_BUFFER, 36 * sizeof(glm::vec3), posVerts.data());
	vao->attach(posData, AttachType::VEC3);

	std::shared_ptr<Buffer> normData = std::make_shared<Buffer>(GL_ARRAY_BUFFER, 36 * sizeof(glm::vec3), normVerts.data());
	vao->attach(normData, AttachType::VEC3);

	std::shared_ptr<Buffer> uvData = std::make_shared<Buffer>(GL_ARRAY_BUFFER, 36 * sizeof(glm::vec2), uvVerts.data());
	vao->attach(uvData, AttachType::VEC2);

	std::shared_ptr<Buffer> minPos = std::make_shared<Buffer>(GL_ARRAY_BUFFER, numInstances * sizeof(glm::vec3), bbMin);
	vao->attach(minPos, AttachType::VEC3, true);

	std::shared_ptr<Buffer> maxPos = std::make_shared<Buffer>(GL_ARRAY_BUFFER, numInstances * sizeof(glm::vec3), bbMax);
	vao->attach(maxPos, AttachType::VEC3, true);

	vao->unbind();

	return vao;
}

std::shared_ptr<VertexArray> generateCircle2D(float radius, unsigned int numSlices, bool filled, float yAspect)
{
	if (numSlices < 3)
	{
		throw std::runtime_error("Error: Cannot generate a circle with less than three slices!");
	}

	std::vector<glm::vec2> verts = std::vector<glm::vec2>();

	float delta = 2.0*_PI_ / float(numSlices);
	float theta = 0.0f;
	if(filled) verts.push_back(glm::vec2(0.0));
	for (int i = 0; i < numSlices + 1; ++i)
	{
		verts.push_back(glm::vec2(cos(theta)*radius, sin(theta)*radius*yAspect));
		theta += delta;
	}

	std::shared_ptr<VertexArray> vao = std::make_shared<VertexArray>();

	vao->bind();
	vao->setVertexSize((filled)? numSlices + 2 : numSlices + 1);
	vao->setPrimativeType((filled)? GL_TRIANGLE_FAN : GL_LINE_STRIP);
	std::shared_ptr<Buffer> posData = std::make_shared<Buffer>(GL_ARRAY_BUFFER, ((filled) ? (numSlices + 2) : (numSlices + 1)) * sizeof(glm::vec2), verts.data());
	vao->attach(posData, AttachType::VEC2);
	vao->unbind();

	return vao;
}

void generateCylinderHelper(std::vector<glm::vec3>* posVerts, std::vector<glm::vec3>* normalVerts, glm::vec3 position, float radius, float height, unsigned int numSlices)
{
	float delta = 2.0*_PI_ / float(numSlices);
	float theta = 0.0f;
	for (int i = 0; i < numSlices + 1; ++i)
	{
		glm::vec2 a = glm::vec2(cos(theta)*radius, sin(theta)*radius);
		glm::vec2 b = glm::vec2(cos(theta + delta)*radius, sin(theta + delta)*radius);

		//add bot
		addTri(posVerts, normalVerts, glm::vec3(b.x, 0.0, b.y) + position, glm::vec3(a.x, 0.0, a.y) + position, glm::vec3(0.0, 0.0, 0.0) + position, glm::vec3(0.0, 1.0, 0.0));
		//add quad
		glm::vec3 normal = glm::normalize(glm::vec3(cos(theta + delta*0.5f)*radius, 0.0, sin(theta + delta*0.5f)*radius));
		addQuad(posVerts, normalVerts, nullptr,
			glm::vec3(b.x, 0.0, b.y) + position, glm::vec3(a.x, 0.0, a.y) + position, glm::vec3(b.x, height, b.y) + position, glm::vec3(a.x, height, a.y) + position, normal);
		//add top
		addTri(posVerts, normalVerts, glm::vec3(a.x, height, a.y) + position, glm::vec3(b.x, height, b.y) + position, glm::vec3(0.0, height, 0.0) + position, glm::vec3(0.0, 1.0, 0.0));

		theta += delta;
	}
}

void generateConeHelper(std::vector<glm::vec3>* posVerts, std::vector<glm::vec3>* normalVerts, glm::vec3 position, float radius, float height, unsigned int numSlices)
{
	float delta = 2.0*_PI_ / float(numSlices);
	float theta = 0.0f;
	for (int i = 0; i < numSlices + 1; ++i)
	{
		glm::vec3 a = glm::vec3(cos(theta)*radius, 0.0f, sin(theta)*radius) + position;
		glm::vec3 b = glm::vec3(cos(theta + delta)*radius, 0.0f, sin(theta + delta)*radius) + position;
		glm::vec3 c = glm::vec3(0.0f, height, 0.0f) + position;

		//add bot
		addTri(posVerts, normalVerts, b, a, c, glm::vec3(0.0, 1.0, 0.0));

		//add top
		glm::vec3 ca = glm::normalize(a - c);
		glm::vec3 cb = glm::normalize(b - c);
		glm::vec3 normal = glm::normalize(glm::cross(ca, cb));
		addTri(posVerts, normalVerts, a, b, c, normal);

		theta += delta;
	}
}

std::shared_ptr<VertexArray> generateCylinder(glm::vec3 position, float radius, float height, unsigned int numSlices)
{
	if (numSlices < 3)
	{
		throw std::runtime_error("Error: Cannot generate a circle with less than three slices!");
	}

	std::vector<glm::vec3> posVerts = std::vector<glm::vec3>();
	std::vector<glm::vec3> normalVerts = std::vector<glm::vec3>();

	generateCylinderHelper(&posVerts, &normalVerts, position, radius, height, numSlices);
	
	std::shared_ptr<VertexArray> vao = std::make_shared<VertexArray>();

	vao->bind();
	vao->setVertexSize((numSlices + 1) * 12);
	std::shared_ptr<Buffer> posData = std::make_shared<Buffer>(GL_ARRAY_BUFFER, (numSlices + 1) * 12 * sizeof(glm::vec3), posVerts.data());
	vao->attach(posData, AttachType::VEC3);
	std::shared_ptr<Buffer> normalData = std::make_shared<Buffer>(GL_ARRAY_BUFFER, (numSlices + 1) * 12 * sizeof(glm::vec3), normalVerts.data());
	vao->attach(normalData, AttachType::VEC3);
	vao->unbind();

	return vao;
}

std::shared_ptr<VertexArray> generateCone(glm::vec3 position, float radius, float height, unsigned int numSlices)
{
	if (numSlices < 3)
	{
		throw std::runtime_error("Error: Cannot generate a circle with less than three slices!");
	}

	std::vector<glm::vec3> posVerts = std::vector<glm::vec3>();
	std::vector<glm::vec3> normalVerts = std::vector<glm::vec3>();

	generateConeHelper(&posVerts, &normalVerts, position, radius, height, numSlices);

	std::shared_ptr<VertexArray> vao = std::make_shared<VertexArray>();

	vao->bind();
	vao->setVertexSize((numSlices + 1) * 6);
	std::shared_ptr<Buffer> posData = std::make_shared<Buffer>(GL_ARRAY_BUFFER, (numSlices + 1) * 6 * sizeof(glm::vec3), posVerts.data());
	vao->attach(posData, AttachType::VEC3);
	std::shared_ptr<Buffer> normalData = std::make_shared<Buffer>(GL_ARRAY_BUFFER, (numSlices + 1) * 6 * sizeof(glm::vec3), normalVerts.data());
	vao->attach(normalData, AttachType::VEC3);
	vao->unbind();

	return vao;
}

std::shared_ptr<VertexArray> generateArrow(float coneRadius, float cylinderRadius, float percentCone, float height, unsigned int numSlices)
{
	if (numSlices < 3)
	{
		throw std::runtime_error("Error: Cannot generate a circle with less than three slices!");
	}

	std::vector<glm::vec3> posVerts = std::vector<glm::vec3>();
	std::vector<glm::vec3> normalVerts = std::vector<glm::vec3>();

	generateCylinderHelper(&posVerts, &normalVerts, glm::vec3(0.0,0.0,0.0), cylinderRadius, height*(1.0 - percentCone), numSlices);
	generateConeHelper(&posVerts, &normalVerts, glm::vec3(0.0, height*(1.0 - percentCone), 0.0), coneRadius, height*percentCone, numSlices);

	std::shared_ptr<VertexArray> vao = std::make_shared<VertexArray>();

	vao->bind();
	vao->setVertexSize((numSlices + 1) * 18);
	std::shared_ptr<Buffer> posData = std::make_shared<Buffer>(GL_ARRAY_BUFFER, (numSlices + 1) * 18 * sizeof(glm::vec3), posVerts.data());
	vao->attach(posData, AttachType::VEC3);
	std::shared_ptr<Buffer> normalData = std::make_shared<Buffer>(GL_ARRAY_BUFFER, (numSlices + 1) * 18 * sizeof(glm::vec3), normalVerts.data());
	vao->attach(normalData, AttachType::VEC3);
	vao->unbind();

	return vao;
}

glm::vec2* scaleVerts2D(glm::vec2* verts, unsigned int numVerts, glm::vec2 scale, glm::vec2 offset)
{
	glm::vec2* newVerts = new glm::vec2[numVerts];
	for (int i = 0; i < numVerts; i+=2)
	{
		newVerts[i] = verts[i] * scale + offset;
	}
	return newVerts;
}

glm::vec3* scaleVerts3D(glm::vec3* verts, unsigned int numVerts, glm::vec3 scale, glm::vec3 offset)
{
	glm::vec3* newVerts = new glm::vec3[numVerts];
	for (int i = 0; i < numVerts; i += 2)
	{
		newVerts[i] = verts[i] * scale + offset;
	}
	return newVerts;
}



void addTri(std::vector<glm::vec3>* positions, std::vector<glm::vec3>* normals, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 normal)
{
	positions->push_back(a);
	positions->push_back(b);
	positions->push_back(c);

	normals->push_back(normal);
	normals->push_back(normal);
	normals->push_back(normal);
}


/*
	C D
	+-+
	| |
	+-+
	A B
*/
void addQuad(std::vector<glm::vec3>* positions, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* texCoords, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, glm::vec3 norm)
{
	positions->push_back(a);
	positions->push_back(d);
	positions->push_back(b);

	positions->push_back(a);
	positions->push_back(c);
	positions->push_back(d);

	if (texCoords != nullptr)
	{
		texCoords->push_back(glm::vec2(1.0, 0.0));
		texCoords->push_back(glm::vec2(0.0, 1.0));
		texCoords->push_back(glm::vec2(0.0, 0.0));

		texCoords->push_back(glm::vec2(1.0, 0.0));
		texCoords->push_back(glm::vec2(1.0, 1.0));
		texCoords->push_back(glm::vec2(0.0, 1.0));
	}

	if (normals != nullptr)
	{
		normals->push_back(norm);
		normals->push_back(norm);
		normals->push_back(norm);

		normals->push_back(norm);
		normals->push_back(norm);
		normals->push_back(norm);
	}
}



BoundingBoxArray::BoundingBoxArray(int maxBoundingBoxes)
{
	glm::vec3 min = glm::vec3(0.0);
	glm::vec3 max = glm::vec3(1.0);
	glm::vec3 scale = max - min;
	glm::vec3 offset = min;

	std::vector<glm::vec3> posVerts = std::vector<glm::vec3>();
	std::vector<glm::vec3> normVerts = std::vector<glm::vec3>();
	std::vector<glm::vec2> uvVerts = std::vector<glm::vec2>();
	addQuad(&posVerts, &normVerts, &uvVerts, glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, min.y, min.z), glm::vec3(min.x, max.y, min.z), glm::vec3(max.x, max.y, min.z), glm::vec3(0.0, 0.0, -1.0));
	addQuad(&posVerts, &normVerts, &uvVerts, glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, min.y, max.z), glm::vec3(max.x, max.y, min.z), glm::vec3(max.x, max.y, max.z), glm::vec3(1.0, 0.0, 0.0));
	addQuad(&posVerts, &normVerts, &uvVerts, glm::vec3(max.x, min.y, max.z), glm::vec3(min.x, min.y, max.z), glm::vec3(max.x, max.y, max.z), glm::vec3(min.x, max.y, max.z), glm::vec3(0.0, 0.0, 1.0));
	addQuad(&posVerts, &normVerts, &uvVerts, glm::vec3(min.x, min.y, max.z), glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, max.y, max.z), glm::vec3(min.x, max.y, min.z), glm::vec3(-1.0, 0.0, 0.0));
	addQuad(&posVerts, &normVerts, &uvVerts, glm::vec3(min.x, max.y, min.z), glm::vec3(max.x, max.y, min.z), glm::vec3(min.x, max.y, max.z), glm::vec3(max.x, max.y, max.z), glm::vec3(0.0, 1.0, 0.0));
	addQuad(&posVerts, &normVerts, &uvVerts, glm::vec3(min.x, min.y, max.z), glm::vec3(max.x, min.y, max.z), glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, min.y, min.z), glm::vec3(0.0, -1.0, 0.0));

	_vao = std::make_shared<VertexArray>();
	_vao->bind();
	_vao->setVertexSize(36);

	std::shared_ptr<Buffer> posData = std::make_shared<Buffer>(GL_ARRAY_BUFFER, 36 * sizeof(glm::vec3), posVerts.data());
	_vao->attach(posData, AttachType::VEC3);

	std::shared_ptr<Buffer> normData = std::make_shared<Buffer>(GL_ARRAY_BUFFER, 36 * sizeof(glm::vec3), normVerts.data());
	_vao->attach(normData, AttachType::VEC3);

	std::shared_ptr<Buffer> uvData = std::make_shared<Buffer>(GL_ARRAY_BUFFER, 36 * sizeof(glm::vec2), uvVerts.data());
	_vao->attach(uvData, AttachType::VEC2);

	_minPos = std::make_shared<Buffer>(GL_ARRAY_BUFFER, maxBoundingBoxes * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);
	_vao->attach(_minPos, AttachType::VEC3, true);

	_maxPos = std::make_shared<Buffer>(GL_ARRAY_BUFFER, maxBoundingBoxes * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);
	_vao->attach(_maxPos, AttachType::VEC3, true);

	_vao->unbind();
}

BoundingBoxArray::~BoundingBoxArray()
{
}

void BoundingBoxArray::update(glm::vec3* bbMin, glm::vec3* bbMax, unsigned int numInstances)
{
	_minPos->update(0, numInstances * sizeof(glm::vec3), bbMin);
	_maxPos->update(0, numInstances * sizeof(glm::vec3), bbMax);
}

std::shared_ptr<VertexArray> BoundingBoxArray::getVAO()
{
	return _vao;
}
