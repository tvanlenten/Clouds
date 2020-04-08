#pragma once

#include "Texture.h"


class Texture2D : public Texture
{
public:
	Texture2D(std::string filename, bool mipmapped = false, bool flipVertically = true);

	/*
	Internal
	-GL_R32F
	-GL_R32I
	-GL_RGBUI
	-GL_RGBA32F

	Format
	-GL_RED
	-GL_RED_INTEGER
	-GL_RGB_INTEGER
	-GL_RGBA

	Type
	-GL_UNSIGNED_BYTE
	-GL_FLOAT
	-GL_UNSIGNED_INT
	-GL_INT
	*/
	Texture2D(void* data, glm::ivec2 dims, GLenum internalFormat, GLenum format, GLenum dataType, bool mipmapped = false);
	~Texture2D();


	void setWrappingInternal();

	void updateSubTexture(glm::ivec2 offset, glm::ivec2 dim, void* data);

	void resize(glm::ivec2 dimensions);
	glm::ivec2 dimensions();
private:
	glm::ivec2 _dimensions;
};