#pragma once
#include <glad/glad.h>
#include <memory>

class Buffer;

enum class AttachType
{
	UBYTE,
	BYTE,
	FLOAT,
	VEC2,
	VEC3,
	VEC4,
	INT,
	IVEC2,
	IVEC3,
	IVEC4,
	UINT,
	UVEC2,
	UVEC3,
	UVEC4
};

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void bind();
	void unbind();

	/*
		be sure to bind the vertex buffer and optional index buffer!
		example for texture coordinates
		attach(GL_FLOAT, 2, 4 * sizeof(GLfloat), 0, false)
	*/
	void attach(GLenum dataType, int numberComponents, int stride, int offset, bool instance = false);
	
	void attach(std::shared_ptr<Buffer> buffer, AttachType attachType, bool instance = false);


	void setInstanceSize(unsigned int size);
	void setVertexSize(unsigned int size);
	void setPrimativeType(GLenum primativeType = GL_TRIANGLES);

	void draw();
	void drawInstanced();
	void drawInstanced(unsigned int numInstances);

private:
	GLuint _id;
	unsigned int _vertexSize;
	unsigned int _instanceSize;
	unsigned int _attachCount;
	GLenum _primativeType;
};

