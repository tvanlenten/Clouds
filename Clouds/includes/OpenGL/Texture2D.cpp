#include "Texture2D.h"
#include "OpenGLDebug.h"

Texture2D::Texture2D(std::string filename, bool mipmapped, bool flipVertically)
{
	_dataType = GL_UNSIGNED_BYTE;
	_mipmapped = mipmapped;
	_type = GL_TEXTURE_2D;
	glGenTextures(1, &_id);
	int numChannels;
	unsigned char* image = loadImage(filename.c_str(), &_dimensions.x, &_dimensions.y, &numChannels, flipVertically);
	bind();
	if (numChannels == 4)
	{
		_internalFormat = GL_RGBA;
		_format = GL_RGBA;
	}
	else
	{
		_internalFormat = GL_RGB;
		_format = GL_RGB;
	}
	glTexImage2D(_type, 0, _internalFormat, _dimensions.x, _dimensions.y, 0, _format, _dataType, image);
	if (_mipmapped)
	{
		glGenerateMipmap(_type); glCheckError();
		setFilter(GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		setFilter(GL_LINEAR);
	}
	setWrapping(GL_CLAMP_TO_EDGE);
	unbind();
	freeImage(image);
}

Texture2D::Texture2D(void* data, glm::ivec2 dims, GLenum internalFormat, GLenum format, GLenum dataType, bool mipmapped)
{
	_dataType = dataType;
	_mipmapped = mipmapped;
	_type = GL_TEXTURE_2D;
	_dimensions = glm::vec2(dims);
	_internalFormat = internalFormat;
	_format = format;
	glGenTextures(1, &_id); glCheckError();

	bind();
	glTexImage2D(_type, 0, _internalFormat, _dimensions.x, _dimensions.y, 0, _format, _dataType, data); glCheckError();
	if (_mipmapped)
	{
		glGenerateMipmap(_type); glCheckError();
		setFilter(GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		setFilter(GL_NEAREST);
	}
	setWrapping(GL_CLAMP_TO_EDGE);
	unbind();
}

Texture2D::~Texture2D()
{
	if (_id) { glDeleteTextures(1, &_id); glCheckError(); }
}

void Texture2D::setWrappingInternal()
{
	glTexParameteri(_type, GL_TEXTURE_WRAP_S, _wrap);
	glTexParameteri(_type, GL_TEXTURE_WRAP_T, _wrap);
	glCheckError();
}

void Texture2D::updateSubTexture(glm::ivec2 offset, glm::ivec2 dim, void * data)
{
	bind();
	glTexSubImage2D(GL_TEXTURE_2D, 0, offset.x, offset.y, dim.x, dim.y, _format, _dataType, data);
	unbind();
}

void Texture2D::resize(glm::ivec2 dimensions)
{
	if (_id != 0)
	{
		bind();
		glTexImage2D(_type, 0, _internalFormat, _dimensions.x, _dimensions.y, 0, _format, _dataType, nullptr); glCheckError();
		if (_mipmapped) { glGenerateMipmap(_type); glCheckError(); }
		setFilterInternal();
		setWrappingInternal();
		unbind();
	}
}

glm::ivec2 Texture2D::dimensions()
{
	return _dimensions;
}
