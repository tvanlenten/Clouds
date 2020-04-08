#pragma once
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

class ShaderBase
{
public:
	virtual ~ShaderBase();

	void Start();
	void End();

	void Set(const char* varName, bool value);
	void Set(const char* varName, int value);
	void Set(const char* varName, float value);
	void Set(const char* varName, const glm::vec2 &value);
	void Set(const char* varName, float x, float y);
	void Set(const char* varName, const glm::vec3 &value);
	void Set(const char* varName, float x, float y, float z);
	void Set(const char* varName, const glm::vec4 &value);
	void Set(const char* varName, float x, float y, float z, float w);
	void Set(const char* varName, const glm::mat2 &mat);
	void Set(const char* varName, const glm::mat3 &mat);
	void Set(const char* varName, const glm::mat4 &mat);
	void Set(const char* varName, const glm::ivec2 &value);
	void Set(const char* varName, const glm::ivec3 &value);
	void Set(const char* varName, const glm::ivec4 &value);

protected:
	GLuint _id;
};