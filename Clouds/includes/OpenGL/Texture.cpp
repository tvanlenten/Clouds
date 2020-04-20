#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "OpenGLDebug.h"

unsigned char* Texture::loadImage(std::string filename, int* width, int* height, int* numChannels, bool flipVertically)
{
	stbi_set_flip_vertically_on_load(flipVertically);
	unsigned char* image = stbi_load(filename.c_str(), width, height, numChannels, 0);
	if (!image)
	{
		printf("Error: Could not load image at: %s\n", filename.c_str());
	}
	return image;
}

float* Texture::loadHeightMap(std::string filename, int* width, int* height, bool flipVertically)
{
	stbi_set_flip_vertically_on_load(flipVertically);
	int numChannels;
	unsigned char* image = stbi_load(filename.c_str(), width, height, &numChannels, 1);
	if (!image)
	{
		printf("Error: Could not load image at: %s\n", filename.c_str());
		return nullptr;
	}

	// convert 8 bit int to float
	float* heightMap = new float[(*width) * (*height)];
	for (int y = 0; y < *height; ++y)
	{
		for (int x = 0; x < *width; ++x)
		{
			int index = x + (y * (*width));
			heightMap[index] = float(image[index]) / 255.0f;
		}
	}
	freeImage(image);

	return heightMap;
}

void Texture::freeImage(void* image)
{
	stbi_image_free(image);
}

void Texture::clear(glm::vec4 color)
{
	glClearTexImage(_id, 0, _format, GL_FLOAT, &color);
}

void Texture::clear(glm::uvec4 value)
{
	glClearTexImage(_id, 0, _format, GL_INT, &value);
}

void Texture::clear()
{
	glClearTexImage(_id, 0, _format, _dataType, nullptr);
}

GLenum Texture::getFormat()
{
	return _format;
}

GLenum Texture::getInternalFormat()
{
	return _internalFormat;
}

GLenum Texture::getType()
{
	return _type;
}

GLenum Texture::getDataType()
{
	return _dataType;
}

void Texture::setFilter(GLenum filter)
{
	_filter = filter;
	setFilterInternal();
}

void Texture::setWrapping(GLenum wrap)
{
	_wrap = wrap;
	setWrappingInternal();
}

void Texture::setFilterInternal()
{
	glTexParameteri(_type, GL_TEXTURE_MIN_FILTER, _filter);
	glTexParameteri(_type, GL_TEXTURE_MAG_FILTER, _filter);
	glCheckError();
}

void Texture::bind()
{
	glBindTexture(_type, _id); glCheckError();
}

void Texture::unbind()
{
	glBindTexture(_type, 0); glCheckError();
}

void Texture::use(GLuint tex)
{
	glActiveTexture(GL_TEXTURE0 + tex);
	glBindTexture(_type, _id); glCheckError();
}

void Texture::bindTo(int bind, GLenum memoryType)
{
	glBindTexture(_type, _id);
	glBindImageTexture(bind, _id, 0, GL_TRUE, 0, memoryType, _internalFormat); glCheckError();
	glBindTexture(_type, 0);
}

GLuint Texture::textureId()
{
	return _id;
}
