#pragma once
#include <glad/glad.h>

#include <glm/glm.hpp>

/*
	OpenGL shortcuts
*/

void DepthTest(GLenum depthTest = GL_LESS);
void CullFace(GLenum cullFace = GL_BACK);
void DisableDepthTest();
void DisableCullFace();
void EnableWireframe();
void DisableWireframe();

void EnableBackFrontAlphaBlending();
void EnableFrontBackAlphaBlending();
void DisableAlphaBlending();

glm::mat3 GetNormalMatrix(glm::mat4 modelMatrix);