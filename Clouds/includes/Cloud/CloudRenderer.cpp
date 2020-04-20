#include "CloudRenderer.h"

#include "../OpenGL/RenderTarget.h"
#include "../OpenGL/VertexArray.h"
#include "../OpenGL/Shader.h"
#include "../OpenGL/Texture2D.h"
#include "../OpenGL/Texture3D.h"
#include "../OpenGL/OpenGLState.h"
#include "../OpenGL/Camera.h"


CloudRenderer::CloudRenderer(glm::ivec2 cloudPassDims, std::shared_ptr<Texture3D> cloudVolume)
{
	_cloudVolume = cloudVolume;

	_cloudShader = std::make_shared<Shader>("shaders/screen.vert", "shaders/cloudRenderer.frag", nullptr, false);
	_cloudShader->Start();
	_cloudShader->Set("cloudVolume", 0);
	_cloudShader->Set("prevDepth", 1);
	_cloudShader->End();
}

CloudRenderer::~CloudRenderer()
{
}

void CloudRenderer::Draw(std::shared_ptr<RenderTarget> target, std::shared_ptr<Camera> camera)
{
	// depth buffer should be read but every fragment should pass thus set to GL_ALWAYS
	// alpha bleding is also enabled since the clouds will be transparent
	DisableDepthTest();
	EnableBackFrontAlphaBlending();

	_cloudShader->Start();
	_cloudShader->Set("invProjView", camera->InverseProjectionViewMatrix());
	_cloudVolume->use(0);
	target->getDepthAttachment()->use(1); //use the depth buffer

	target->renderToTarget(); // draw screen quad
	_cloudShader->End();

	// reset back to default draw state!
	DisableAlphaBlending();
	DepthTest();
}

