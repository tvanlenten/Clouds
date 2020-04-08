#include "Texture3D.h"
#include "OpenGLDebug.h"

Texture3D::Texture3D(void * data, glm::ivec3 dims, GLenum internalFormat, GLenum format, GLenum dataType, bool mipmapped)
{
	_dataType = dataType;
	_mipmapped = mipmapped;
	_type = GL_TEXTURE_3D;
	_internalFormat = internalFormat;
	_format = format;
	_dimensions = dims;
	glGenTextures(1, &_id); glCheckError();

	bind();
	glTexImage3D(_type, 0, _internalFormat, _dimensions.x, _dimensions.y, _dimensions.z, 0, _format, _dataType, data); glCheckError();
	if (_mipmapped)glGenerateMipmap(_type);
	setFilter(GL_NEAREST);
	setWrapping(GL_CLAMP_TO_EDGE);
	unbind();
}

Texture3D::~Texture3D()
{
	if (_id) { glDeleteTextures(1, &_id); glCheckError(); }
}

void Texture3D::setWrappingInternal()
{
	glTexParameteri(_type, GL_TEXTURE_WRAP_S, _wrap);
	glTexParameteri(_type, GL_TEXTURE_WRAP_T, _wrap);
	glTexParameteri(_type, GL_TEXTURE_WRAP_R, _wrap);
	glCheckError();
}

void Texture3D::updateSubTexture(glm::ivec3 offset, glm::ivec3 dim, void* data)
{
	if (_id != 0)
	{
		bind();
		glTexSubImage3D(GL_TEXTURE_3D, 0, offset.x, offset.y, offset.z, dim.x, dim.y, dim.z, _format, _dataType, data);
		unbind();
	}
}

void Texture3D::resize(glm::ivec3 dimensions)
{
	if (_id != 0)
	{
		bind();
		glTexImage3D(_type, 0, _internalFormat, _dimensions.x, _dimensions.y, _dimensions.z, 0, _format, _dataType, nullptr); glCheckError();
		if (_mipmapped) { glGenerateMipmap(_type); glCheckError(); }
		setFilterInternal();
		setWrappingInternal();
		unbind();
	}
}

glm::ivec3 Texture3D::dimensions()
{
	return _dimensions;
}

float Texture3D::voxelSize()
{
	float maxDim = glm::max(glm::max(_dimensions.x, _dimensions.y), _dimensions.z);
	return 1.0f / maxDim;
}
