#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>


class Texture
{
public:
	virtual ~Texture() {}

	void bind();
	void unbind();
	void use(GLuint tex);
	void bindTo(int bind, GLenum memoryType = GL_WRITE_ONLY);

	void setFilter(GLenum filter);
	void setWrapping(GLenum wrap);

	void setFilterInternal();
	virtual void setWrappingInternal() = 0;

	/*
	loads an image from the stbi library
	*/
	unsigned char* loadImage(std::string filename, int* width, int* height, int* numChannels, bool flipVertically = true);

	/*
	free the image data after using!
	*/
	void freeImage(unsigned char* image);

	void clear(glm::vec4 color);
	void clear(glm::uvec4 value);
	void clear();

	GLenum getFormat();
	GLenum getInternalFormat();
	GLenum getType();
	GLenum getDataType();

	GLuint textureId();
protected:
	GLuint _id;
	GLenum _type;
	GLenum _format;
	GLenum _internalFormat;
	GLenum _dataType;
	GLenum _filter;
	GLenum _wrap;
	bool _mipmapped;

};