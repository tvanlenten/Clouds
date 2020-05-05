#include "CloudRenderer.h"

#include "../OpenGL/RenderTarget.h"
#include "../OpenGL/VertexArray.h"
#include "../OpenGL/Shader.h"
#include "../OpenGL/Texture2D.h"
#include "../OpenGL/Texture3D.h"
#include "../OpenGL/OpenGLState.h"
#include "../OpenGL/Camera.h"
#include "../OpenGL/TextureCubeMap.h"

#include "../ImGui/imgui.h"


CloudRenderer::CloudRenderer(glm::ivec2 cloudPassDims, std::shared_ptr<Texture3D> cloudVolume)
{
	_cloudVolume = cloudVolume;

	_cloudShader = std::make_shared<Shader>("shaders/screen.vert", "shaders/cloudRenderer.frag", nullptr, false);
	_cloudShader->Start();
	_cloudShader->Set("cloudVolume", 0);
	_cloudShader->Set("skybox", 1);
	_cloudShader->Set("prevColorTex", 2);
	_cloudShader->Set("prevDepthTex", 3);
	_cloudShader->End();

	_cloudTarget = std::make_shared<RenderTarget>(cloudPassDims);
	_cloudTarget->addColorAttachment();
	_cloudTarget->bindAttachments();

	_rayMarchstepSize = 0.1;
}

CloudRenderer::~CloudRenderer()
{
}

void CloudRenderer::Draw(std::shared_ptr<RenderTarget> target, std::shared_ptr<Camera> camera, std::shared_ptr<TextureCubeMap> skybox, glm::vec3 sunDir, float sunPower, float time)
{
	// depth buffer should be read but every fragment should pass thus set to GL_ALWAYS
	// alpha bleding is also enabled since the clouds will be transparent
	DisableDepthTest();
	//EnableBackFrontAlphaBlending();

	_cloudTarget->bind();

	_cloudShader->Start();
	_cloudShader->Set("invProjView", camera->InverseProjectionViewMatrix());
	_cloudShader->Set("sunDir", glm::normalize(sunDir));
	_cloudShader->Set("sunPower", sunPower);
	_cloudShader->Set("stepSize", _rayMarchstepSize);
	_cloudShader->Set("time", time);

	// Vary Cloud shapes and sizes
	_cloudShader->Set("xScale", _xScale_DEBUG);
	_cloudShader->Set("yScale", _yScale_DEBUG);
	_cloudShader->Set("zScale", _zScale_DEBUG);
	_cloudShader->Set("cloudDefinition", _cloudDefinition_DEBUG);
	_cloudShader->Set("cloudTrim", _cloudTrim_DEBUG);
	_cloudShader->Set("cloudHeight", _cloudHeight_DEBUG);
	_cloudShader->Set("cloudSlice", _cloudSlice_DEBUG);
	_cloudShader->Set("cloudDensity", _cloudDensity_DEBUG);

	_cloudVolume->use(0);
	skybox->use(1);
	target->getColorAttachment(0)->use(2); //use the prev color buffer
	target->getDepthAttachment()->use(3); //use the prev depth buffer

	_cloudTarget->renderToTarget(); // draw screen quad
	_cloudShader->End();

	// reset back to default draw state!
	//DisableAlphaBlending();
	DepthTest();

	target->blitToTarget(_cloudTarget, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	target->bind();
}

void CloudRenderer::Gui()
{
	ImGui::DragFloat("xScale", &_xScale_DEBUG, 0.001f, 0.025f, 0.25f);
	ImGui::DragFloat("yScale", &_yScale_DEBUG, 0.001f, 0.025f, 0.25f);
	ImGui::DragFloat("zScale", &_zScale_DEBUG, 0.001f, 0.025f, 0.25f);
	ImGui::DragFloat("cloudDefinition", &_cloudDefinition_DEBUG, 0.001f, 0.0f, 1.0f);
	ImGui::DragFloat("cloudTrim", &_cloudTrim_DEBUG, 0.001f, 0.0f, 1.0f);
	ImGui::DragFloat("cloudHeight", &_cloudHeight_DEBUG, 0.01f, 3.0f, 12.0f);
	ImGui::DragFloat("cloudSlice", &_cloudSlice_DEBUG, 0.001f, 0.0f, 3.5f);
	ImGui::DragFloat("cloudDensity", &_cloudDensity_DEBUG, 0.01f, 0.0f, 10.0f);
}

void CloudRenderer::SetCloudVolume(std::shared_ptr<Texture3D> cloudVolume)
{
	_cloudVolume = cloudVolume;
}
