#pragma once

#include "Texture.h"

class Texture3D : public Texture
{
public:
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
	Texture3D(void* data, glm::ivec3 dims, GLenum internalFormat, GLenum format, GLenum dataType, bool mipmapped = false);
	~Texture3D();

	void setWrappingInternal();

	void updateSubTexture(glm::ivec3 offset, glm::ivec3 dim, void* data);

	void resize(glm::ivec3 dimensions);
	glm::ivec3 dimensions();
	float voxelSize();
private:
	glm::ivec3 _dimensions;
};