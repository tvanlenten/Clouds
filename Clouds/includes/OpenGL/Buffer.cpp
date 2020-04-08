#include "Buffer.h"
#include "OpenGLDebug.h"

Buffer::Buffer(GLenum type, unsigned int dataSize, GLvoid * data, GLenum memoryType)
{
	_type = type;
	glGenBuffers(1, &_id); glCheckError();
	glBindBuffer(_type, _id); glCheckError();
	glBufferData(_type, dataSize, data, memoryType); glCheckError();
	glBindBuffer(_type, 0); glCheckError();
}

void Buffer::update(int offset, int size, GLvoid * data)
{
	glBindBuffer(_type, _id);
	glBufferSubData(_type, offset, size, data); glCheckError();
	glBindBuffer(_type, 0);
}

void Buffer::bind()
{
	glBindBuffer(_type, _id); glCheckError();
}

void Buffer::bindTo(int base)
{
	bind();
	glBindBufferBase(_type, base, _id); glCheckError();
}

void Buffer::unbind()
{
	glBindBuffer(_type, 0); glCheckError();
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &_id); glCheckError();
}
