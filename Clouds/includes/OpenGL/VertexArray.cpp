#include "VertexArray.h"
#include "OpenGLDebug.h"
#include "Buffer.h"


VertexArray::VertexArray()
{
	_vertexSize = 0;
	_instanceSize = 0;
	_attachCount = 0;
	_primativeType = GL_TRIANGLES;
	glGenVertexArrays(1, &_id); glCheckError();
}


VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &_id); glCheckError();
}

void VertexArray::bind()
{
	glBindVertexArray(_id); glCheckError();
}

void VertexArray::unbind()
{
	glBindVertexArray(0); glCheckError();
}

void VertexArray::attach(GLenum dataType, int numberComponents, int stride, int offset, bool instance)
{
	if (dataType == GL_FLOAT)
	{
		glVertexAttribPointer(_attachCount, numberComponents, dataType, GL_FALSE, stride, (GLvoid*)offset); glCheckError();
	}
	else
	{
		glVertexAttribIPointer(_attachCount, numberComponents, dataType, stride, (GLvoid*)offset); glCheckError();
	}
	glEnableVertexAttribArray(_attachCount); glCheckError();
	if (instance) { glVertexAttribDivisor(_attachCount, 1); glCheckError(); }
	_attachCount++;
}

void VertexArray::attach(std::shared_ptr<Buffer> buffer, AttachType attachType, bool instance)
{
	unsigned int offset = 0;
	buffer->bind();
	switch(attachType)
	{
		case AttachType::UBYTE: attach(GL_UNSIGNED_BYTE, 1, 1, offset, instance); break;
		case AttachType::BYTE: attach(GL_BYTE, 1, 1, offset, instance); break;
		case AttachType::FLOAT: attach(GL_FLOAT, 1, sizeof(GLfloat), offset, instance); break;
		case AttachType::VEC2: attach(GL_FLOAT, 2, sizeof(GLfloat) * 2, offset, instance); break;
		case AttachType::VEC3: attach(GL_FLOAT, 3, sizeof(GLfloat) * 3, offset, instance); break;
		case AttachType::VEC4: attach(GL_FLOAT, 4, sizeof(GLfloat) * 4, offset, instance); break;
		case AttachType::INT: attach(GL_INT, 1, sizeof(GLint), offset, instance); break;
		case AttachType::IVEC2: attach(GL_INT, 2, sizeof(GLint) * 2, offset, instance); break;
		case AttachType::IVEC3: attach(GL_INT, 3, sizeof(GLint) * 3, offset, instance); break;
		case AttachType::IVEC4: attach(GL_INT, 4, sizeof(GLint) * 4, offset, instance); break;
		case AttachType::UINT: attach(GL_UNSIGNED_INT, 1, sizeof(GLint), offset, instance); break;
		case AttachType::UVEC2: attach(GL_UNSIGNED_INT, 2, sizeof(GLint) * 2, offset, instance); break;
		case AttachType::UVEC3: attach(GL_UNSIGNED_INT, 3, sizeof(GLint) * 3, offset, instance); break;
		case AttachType::UVEC4: attach(GL_UNSIGNED_INT, 4, sizeof(GLint) * 4, offset, instance); break;
	}
}

void VertexArray::setInstanceSize(unsigned int size)
{
	_instanceSize = size;
}

void VertexArray::setVertexSize(unsigned int size)
{
	_vertexSize = size;
}

void VertexArray::setPrimativeType(GLenum primativeType)
{
	_primativeType = primativeType;
}

void VertexArray::draw()
{
	glBindVertexArray(_id);
	glDrawArrays(_primativeType, 0, _vertexSize); glCheckError();
	glBindVertexArray(0);
}

void VertexArray::drawInstanced()
{
	glBindVertexArray(_id);
	glDrawArraysInstanced(_primativeType, 0, _vertexSize, _instanceSize); glCheckError();
	glBindVertexArray(0);
}

void VertexArray::drawInstanced(unsigned int numInstances)
{
	glBindVertexArray(_id);
	glDrawArraysInstanced(_primativeType, 0, _vertexSize, numInstances); glCheckError();
	glBindVertexArray(0);
}
