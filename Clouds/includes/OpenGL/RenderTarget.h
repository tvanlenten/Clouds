#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <glad/glad.h>

class Texture2D;
class VertexArray;

class RenderTarget
{
public:
	RenderTarget(glm::ivec2 dimensions);
	~RenderTarget();

	void addColorAttachment();
	void addColorAttachment(std::shared_ptr<Texture2D> color);
	void addDepthAttachment(bool depthStencil = false);
	void addDepthAttachment(std::shared_ptr<Texture2D> depth, bool depthStencil = false);
	void addRenderBuffer(bool depthStencil);
	void bindAttachments();
	void clearAttachments();

	std::shared_ptr<Texture2D> getColorAttachment(unsigned int colorAttachId);
	std::shared_ptr<Texture2D> getDepthAttachment();
	glm::ivec2 getDimensions();
	unsigned int getFrameBufferId();

	void renderToTarget();

	void resize(glm::ivec2 dimensions);

	void readPixel(glm::ivec2 coord, unsigned int colorAttachId, void* pixelData);

	/*
		blits src renderTarget onto dest
		mask: GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT
		filter: GL_LINEAR or GL_NEAREST
	*/
	void blitToTarget(std::shared_ptr<RenderTarget> src, GLbitfield mask, GLenum filter);

	void bind();
	void unbind();

	void clear();
	void clearColor(glm::vec4 color = glm::vec4(0.0));
	void clearDepth(float depth = 1.0f);
	void clearStencil(int stencil = 0x00);

private:
	void createRenderBuffer();
private:
	std::shared_ptr<Texture2D> _colorAttachments[4];
	std::shared_ptr<Texture2D> _depthAttachment;
	std::shared_ptr<VertexArray> _screenQuad;
	GLuint _fboID;
	GLuint _rboID;
	bool _depthStencil;
	unsigned int _colorAttachmentCount;
	glm::ivec2 _dimensions;
	bool _bound;
};

