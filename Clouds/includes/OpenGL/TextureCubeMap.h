#pragma once

#include "Texture.h"


class TextureCubeMap : public Texture
{
public:

	TextureCubeMap(std::string faces[6], bool mipmapped = false, bool flipVertically = true);

	TextureCubeMap(glm::ivec2 dims, GLenum internalFormat, GLenum format, GLenum dataType, bool mipmapped = false);

	~TextureCubeMap();

	void setWrappingInternal();

	glm::ivec2 dimensions();
private:
	glm::ivec2 _dimensions;
};