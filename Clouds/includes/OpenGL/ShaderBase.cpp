#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include "OpenGLDebug.h"

ShaderBase::~ShaderBase()
{
	if (_id)
	{
		glDeleteProgram(_id); glCheckError();
	}
}

void ShaderBase::Start()
{
	glUseProgram(_id); glCheckError();
}

void ShaderBase::End()
{
	glUseProgram(0); glCheckError();
}

void ShaderBase::Set(const char* varName, bool value)
{
	glUniform1i(glGetUniformLocation(_id, varName), (int)value); glCheckError();
}

void ShaderBase::Set(const char* varName, int value)
{
	glUniform1i(glGetUniformLocation(_id, varName), value); glCheckError();
}

void ShaderBase::Set(const char* varName, float value)
{
	glUniform1f(glGetUniformLocation(_id, varName), value); glCheckError();
}

void ShaderBase::Set(const char* varName, const glm::vec2 & value)
{
	glUniform2fv(glGetUniformLocation(_id, varName), 1, &value[0]); glCheckError();
}

void ShaderBase::Set(const char* varName, float x, float y)
{
	glUniform2f(glGetUniformLocation(_id, varName), x, y); glCheckError();
}

void ShaderBase::Set(const char* varName, const glm::vec3 & value)
{
	glUniform3fv(glGetUniformLocation(_id, varName), 1, &value[0]); glCheckError();
}

void ShaderBase::Set(const char* varName, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(_id, varName), x, y, z); glCheckError();
}

void ShaderBase::Set(const char* varName, const glm::vec4 & value)
{
	glUniform4fv(glGetUniformLocation(_id, varName), 1, &value[0]); glCheckError();
}

void ShaderBase::Set(const char* varName, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(_id, varName), x, y, z, w); glCheckError();
}

void ShaderBase::Set(const char* varName, const glm::mat2 & mat)
{
	glUniformMatrix2fv(glGetUniformLocation(_id, varName), 1, GL_FALSE, &mat[0][0]); glCheckError();
}

void ShaderBase::Set(const char* varName, const glm::mat3 & mat)
{
	glUniformMatrix3fv(glGetUniformLocation(_id, varName), 1, GL_FALSE, &mat[0][0]); glCheckError();
}

void ShaderBase::Set(const char* varName, const glm::mat4 & mat)
{
	glUniformMatrix4fv(glGetUniformLocation(_id, varName), 1, GL_FALSE, &mat[0][0]); glCheckError();
}

void ShaderBase::Set(const char* varName, const glm::ivec2& value)
{
	glUniform2iv(glGetUniformLocation(_id, varName), 1, &value[0]); glCheckError();
}

void ShaderBase::Set(const char* varName, const glm::ivec3& value)
{
	glUniform3iv(glGetUniformLocation(_id, varName), 1, &value[0]); glCheckError();
}

void ShaderBase::Set(const char* varName, const glm::ivec4& value)
{
	glUniform4iv(glGetUniformLocation(_id, varName), 1, &value[0]); glCheckError();
}