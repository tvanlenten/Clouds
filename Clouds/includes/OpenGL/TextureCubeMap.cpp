#include "TextureCubeMap.h"
#include "OpenGLDebug.h"

TextureCubeMap::TextureCubeMap(std::string faces[6], bool mipmapped, bool flipVertically)
{
	_dimensions = glm::ivec2(0);
	_dataType = GL_UNSIGNED_BYTE;
	_mipmapped = mipmapped;
	_type = GL_TEXTURE_CUBE_MAP;
	_internalFormat = GL_RGBA;
	_format = GL_RGBA;
	glGenTextures(1, &_id); glCheckError();

	bind();
	for (GLuint i = 0; i < 6; i++) {
		int numChannels;
		unsigned char* image = loadImage(faces[i].c_str(), &_dimensions.x, &_dimensions.y, &numChannels, flipVertically);
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
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, _internalFormat, _dimensions.x, _dimensions.y, 0, _format, _dataType, image); glCheckError();
		freeImage(image);
	}
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
}

TextureCubeMap::~TextureCubeMap()
{
	if (_id) { glDeleteTextures(1, &_id); glCheckError(); }
}

void TextureCubeMap::setWrappingInternal()
{
	glTexParameteri(_type, GL_TEXTURE_WRAP_S, _wrap);
	glTexParameteri(_type, GL_TEXTURE_WRAP_T, _wrap);
	glTexParameteri(_type, GL_TEXTURE_WRAP_R, _wrap);
	glCheckError();
}

glm::ivec2 TextureCubeMap::dimensions()
{
	return _dimensions;
}
