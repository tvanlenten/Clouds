#include "CloudRenderer.h"

#include "../OpenGL/RenderTarget.h"
#include "../OpenGL/VertexArray.h"
#include "../OpenGL/Shader.h"

CloudRenderer::CloudRenderer()
{
}

CloudRenderer::~CloudRenderer()
{
}

std::shared_ptr<RenderTarget> CloudRenderer::Draw(std::shared_ptr<RenderTarget> target, glm::mat4 invProjViewMatrix)
{
	return target;
}

