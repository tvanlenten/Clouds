#include "RenderTarget.h"
#include "VertexArray.h"
#include "Texture2D.h"
#include "Buffer.h"
#include "OpenGLDebug.h"
#include <iostream>

RenderTarget::RenderTarget(glm::ivec2 dimensions)
{
	_bound = false;
	_dimensions = dimensions;
	_fboID = 0;
	_rboID = 0;
	clearAttachments();
	GLfloat rectVerts[] = 
	{
		1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,

		1.0f,  1.0f,  1.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,
	};
	_screenQuad = std::make_unique<VertexArray>();
	_screenQuad->bind();
	Buffer meshData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), rectVerts, GL_STATIC_DRAW);
	meshData.bind();
	_screenQuad->setVertexSize(6);
	_screenQuad->attach(GL_FLOAT, 2, 4 * sizeof(GLfloat), 0, false);
	_screenQuad->attach(GL_FLOAT, 2, 4 * sizeof(GLfloat), 2 * sizeof(GLfloat), false);
	_screenQuad->unbind();
}


RenderTarget::~RenderTarget()
{
	clearAttachments();
	if (_fboID) { glDeleteFramebuffers(1, &_fboID); glCheckError(); }

}

void RenderTarget::addColorAttachment()
{
	if (_colorAttachmentCount < 3)
	{
		std::shared_ptr<Texture2D> color = std::make_shared<Texture2D>(nullptr, _dimensions, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, false);
		_colorAttachments[_colorAttachmentCount++] = color;
	}
}

void RenderTarget::addColorAttachment(std::shared_ptr<Texture2D> color)
{
	if(_colorAttachmentCount < 3)_colorAttachments[_colorAttachmentCount++] = color;
}

void RenderTarget::addDepthAttachment(bool depthStencil)
{
	if (_rboID) { glDeleteRenderbuffers(1, &_rboID); glCheckError(); }
	_rboID = 0;
	std::shared_ptr<Texture2D> depth = std::make_shared<Texture2D>(nullptr, _dimensions, 
		(_depthStencil) ? GL_DEPTH24_STENCIL8 : GL_DEPTH_COMPONENT32F, 
		(_depthStencil) ? GL_DEPTH_STENCIL : GL_DEPTH_COMPONENT, GL_FLOAT, false);
	_depthAttachment = depth;
}

void RenderTarget::addDepthAttachment(std::shared_ptr<Texture2D> depth, bool depthStencil)
{
	if (_rboID) { glDeleteRenderbuffers(1, &_rboID); glCheckError(); }
	_rboID = 0;
	_depthAttachment = depth;
	_depthStencil = depthStencil;
}

void RenderTarget::addRenderBuffer(bool depthStencil)
{
	_depthAttachment = nullptr;
	_depthStencil = depthStencil;
	createRenderBuffer();
}

void RenderTarget::bindAttachments()
{
	if (_fboID == 0)
	{
		glGenFramebuffers(1, &_fboID); glCheckError();
	}
	bind();

	if (_colorAttachmentCount > 0)
	{
		for (int i = 0; i < _colorAttachmentCount; i++)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _colorAttachments[i]->textureId(), 0); glCheckError();
		}
		unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 }; glCheckError();
		glDrawBuffers(_colorAttachmentCount, attachments);
	}
	else
	{
		glDrawBuffer(GL_NONE); glCheckError();
		glReadBuffer(GL_NONE); glCheckError();
	}

	if (_depthAttachment != nullptr)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, (_depthStencil) ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthAttachment->textureId(), 0); glCheckError();
	}
	else if (_rboID)
	{
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, (_depthStencil) ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _rboID); glCheckError();
	}

	unbind();
}

void RenderTarget::clearAttachments()
{
	if (_rboID) { glDeleteRenderbuffers(1, &_rboID); glCheckError(); }
	_colorAttachmentCount = 0;
	_rboID = 0;
	_depthStencil = false;
	_colorAttachments[0] = nullptr;
	_colorAttachments[1] = nullptr;
	_colorAttachments[2] = nullptr;
	_colorAttachments[3] = nullptr;
	_depthAttachment = nullptr;
}

std::shared_ptr<Texture2D> RenderTarget::getColorAttachment(unsigned int colorAttachId)
{
	return (colorAttachId < 4)? _colorAttachments[colorAttachId] : nullptr;
}

std::shared_ptr<Texture2D> RenderTarget::getDepthAttachment()
{
	return _depthAttachment;
}

glm::ivec2 RenderTarget::getDimensions()
{
	return _dimensions;
}

unsigned int RenderTarget::getFrameBufferId()
{
	return _fboID;
}

void RenderTarget::renderToTarget()
{
	if(!_bound)bind();
	_screenQuad->draw();
}

void RenderTarget::resize(glm::ivec2 dimensions)
{
	_dimensions = dimensions;
	for (int i = 0; i < _colorAttachmentCount; i++)
	{
		_colorAttachments[i]->resize(dimensions);
	}
	if(_depthAttachment)_depthAttachment->resize(dimensions);

	if (_rboID)addRenderBuffer(_depthStencil);
}

void RenderTarget::readPixel(glm::ivec2 coord, unsigned int colorAttachId, void* pixelData)
{
	if (_colorAttachments[colorAttachId] == nullptr)
	{
		throw std::runtime_error("ERROR: ColorAttachment does not exist!");
	}

	bind();
	glReadBuffer(GL_COLOR_ATTACHMENT0 + colorAttachId);
	glReadPixels(coord.x, coord.y, 1, 1, _colorAttachments[colorAttachId]->getFormat(), _colorAttachments[colorAttachId]->getDataType(), &pixelData);
	glReadBuffer(GL_NONE);
	unbind();
}

void RenderTarget::blitToTarget(std::shared_ptr<RenderTarget> src, GLbitfield mask, GLenum filter)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, src->getFrameBufferId());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fboID);
	glBlitFramebuffer(0, 0, src->getDimensions().x, src->getDimensions().y, 0, 0, _dimensions.x, _dimensions.y, mask, filter);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void RenderTarget::bind()
{
	_bound = true;
	glViewport(0, 0, _dimensions.x, _dimensions.y); glCheckError();
	glBindFramebuffer(GL_FRAMEBUFFER, _fboID); glCheckError();
}

void RenderTarget::unbind()
{
	_bound = false;
	glBindFramebuffer(GL_FRAMEBUFFER, 0); glCheckError();
}

void RenderTarget::clear()
{
	if (!_bound)bind();
	(_depthStencil)? 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT) : 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); glCheckError();
}

void RenderTarget::clearColor(glm::vec4 color)
{
	if (!_bound)bind();
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT); glCheckError();
}

void RenderTarget::clearDepth(float depth)
{
	if (!_bound)bind();
	glClearDepth(depth);
	glClear(GL_DEPTH_BUFFER_BIT); glCheckError();
}

void RenderTarget::clearStencil(int stencil)
{
	if (!_bound)bind();
	glClearStencil(stencil);
	glClear(GL_STENCIL_BUFFER_BIT); glCheckError();
}

void RenderTarget::createRenderBuffer()
{
	if (_rboID) { glDeleteRenderbuffers(1, &_rboID); glCheckError(); }

	bind();
	glGenRenderbuffers(1, &_rboID); glCheckError();
	glBindRenderbuffer(GL_RENDERBUFFER, _rboID); glCheckError();
	glRenderbufferStorage(GL_RENDERBUFFER, (_depthStencil) ? GL_DEPTH24_STENCIL8 : GL_DEPTH_COMPONENT32F, _dimensions.x, _dimensions.y); glCheckError();
	glBindRenderbuffer(GL_RENDERBUFFER, 0); glCheckError();
	unbind();
}
