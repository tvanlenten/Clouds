#pragma once
#include <glad/glad.h>

class Buffer
{
public:
	/*
		GL_ARRAY_BUFFER					Vertex attributes
		GL_ATOMIC_COUNTER_BUFFER		Atomic counter storage
		GL_COPY_READ_BUFFER				Buffer copy source
		GL_COPY_WRITE_BUFFER			Buffer copy destination
		GL_DISPATCH_INDIRECT_BUFFER		Indirect compute dispatch commands
		GL_DRAW_INDIRECT_BUFFER			Indirect command arguments
		GL_ELEMENT_ARRAY_BUFFER			Vertex array indices
		GL_PIXEL_PACK_BUFFER			Pixel read target
		GL_PIXEL_UNPACK_BUFFER			Texture data source
		GL_QUERY_BUFFER					Query result buffer
		GL_SHADER_STORAGE_BUFFER		Read-write storage for shaders
		GL_TEXTURE_BUFFER				Texture data buffer
		GL_TRANSFORM_FEEDBACK_BUFFER	Transform feedback buffer
		GL_UNIFORM_BUFFER				Uniform block storage
	*/
	Buffer(GLenum type, unsigned int dataSize, GLvoid* data, GLenum memoryType = GL_STATIC_DRAW);

	void update(int offset, int size, GLvoid* data);
	void bind();
	void bindTo(int base);
	void unbind();

	~Buffer();
private:
	GLuint _id;
	GLenum _type;
};

