#pragma once
#include "ShaderBase.h"


class Shader : public ShaderBase
{
public:
	//empty shader
	Shader();

	//default shader
	Shader(const char* vertexPath, const char* fragPath, const char* geoPath, bool debug);

	~Shader();

	bool CompileFromCode(const char* vertexCode, const char* fragCode, const char* geoCode);
	bool CompileFromPath(const char* vertexPath, const char* fragPath, const char* geoPath);

};

