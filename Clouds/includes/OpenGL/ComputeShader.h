#pragma once
#include "Shader.h"


class ComputeShader : public Shader
{
public:
	//empty compute shader
	ComputeShader();

	//compute shader
	ComputeShader(const char* codeString, bool debug);

	~ComputeShader();

	bool CompileFromCode(const char* codeString);
	bool CompileFromPath(const char* fileName);

	void Dispatch(glm::ivec3 numGroups);

};

