#include "OpenGLState.h"
#include "OpenGLDebug.h"

void DepthTest(GLenum depthTest)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(depthTest);
}

void CullFace(GLenum cullFace)
{
	glEnable(GL_CULL_FACE);
	glCullFace(cullFace);
}

void DisableDepthTest()
{
	glDisable(GL_DEPTH_TEST);
}

void DisableCullFace()
{
	glDisable(GL_CULL_FACE);
}

void EnableWireframe()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void DisableWireframe()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void EnableBackFrontAlphaBlending()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/*
	background must be initialized to (0,0,0,0)
	multiply each frags RGB by it's alpha	
*/
void EnableFrontBackAlphaBlending()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ONE);
}

void DisableAlphaBlending()
{
	glDisable(GL_BLEND);
}

glm::mat3 GetNormalMatrix(glm::mat4 modelMatrix)
{
	return glm::mat3(glm::transpose(glm::inverse(modelMatrix)));
}
