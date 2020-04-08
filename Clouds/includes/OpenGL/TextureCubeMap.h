#pragma once

#include "Texture.h"


class TextureCubeMap : public Texture
{
public:
	TextureCubeMap(std::string faces[6], bool mipmapped = false, bool flipVertically = true);
	~TextureCubeMap();

	void setWrappingInternal();

	glm::ivec2 dimensions();
private:
	glm::ivec2 _dimensions;
};