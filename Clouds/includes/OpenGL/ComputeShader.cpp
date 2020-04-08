#include "ComputeShader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "OpenGLDebug.h"


ComputeShader::ComputeShader()
{
	_id = 0;
}

ComputeShader::ComputeShader(const char* computePath, bool debug)
{
	std::ifstream computeFile;
	std::string computeCode;

	//read compute file
	computeFile.open(computePath);
	if (computeFile.is_open())
	{
		std::stringstream computeStream;
		computeStream << computeFile.rdbuf();
		computeCode = computeStream.str();
	}
	else
	{
		std::cout << "ERROR: Can't open compute shader file at: " << computePath << std::endl;
	}
	computeFile.close();

	//cast string into array of GLchar
	const GLchar* cCode = computeCode.c_str();

	if (debug)std::cout << cCode << std::endl << std::endl;

	GLuint compute;
	GLint success;
	GLchar infoLog[512];


	//compile vertex shader
	compute = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compute, 1, &cCode, NULL);
	glCompileShader(compute); glCheckError();
	glGetShaderiv(compute, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(compute, 512, NULL, infoLog);
		std::cout << "ERROR: Compute shader COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "Compiled Compute Shader: " << computePath << std::endl;
	}

	// link shaders to program
	_id = glCreateProgram();
	glAttachShader(_id, compute);
	glLinkProgram(_id); glCheckError();
	glGetProgramiv(_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(_id, 512, NULL, infoLog);
		std::cout << "ERROR: Shader program LINKING_FAILED\n" << infoLog << std::endl;
	}

	//delete shaders
	glDeleteShader(compute); glCheckError();
}

ComputeShader::~ComputeShader()
{
	// ShaderBase cleans up
}

bool ComputeShader::CompileFromCode(const char* codeString)
{
	if (_id)
	{
		glDeleteProgram(_id); glCheckError();
	}

	GLuint compute;
	GLint success;
	GLchar infoLog[512];

	//compile vertex shader
	compute = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compute, 1, &codeString, NULL);
	glCompileShader(compute); glCheckError();
	glGetShaderiv(compute, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(compute, 512, NULL, infoLog);
		std::cout << "ERROR: Compute shader COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}

	// link shaders to program
	_id = glCreateProgram();
	glAttachShader(_id, compute);
	glLinkProgram(_id); glCheckError();
	glGetProgramiv(_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(_id, 512, NULL, infoLog);
		std::cout << "ERROR: Shader program LINKING_FAILED\n" << infoLog << std::endl;
		return false;
	}

	//delete shaders
	glDeleteShader(compute); glCheckError();

	return true;
}

bool ComputeShader::CompileFromPath(const char* fileName)
{
	if (_id)
	{
		glDeleteProgram(_id); glCheckError();
	}

	std::ifstream computeFile;
	std::string computeCode;

	//read compute file
	computeFile.open(fileName);
	if (computeFile.is_open())
	{
		std::stringstream computeStream;
		computeStream << computeFile.rdbuf();
		computeCode = computeStream.str();
	}
	else
	{
		std::cout << "ERROR: Can't open compute shader file at: " << fileName << std::endl;
		return false;
	}
	computeFile.close();

	//cast string into array of GLchar
	const GLchar* cCode = computeCode.c_str();

	GLuint compute;
	GLint success;
	GLchar infoLog[512];

	//compile vertex shader
	compute = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compute, 1, &cCode, NULL);
	glCompileShader(compute); glCheckError();
	glGetShaderiv(compute, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(compute, 512, NULL, infoLog);
		std::cout << "ERROR: Compute shader COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}

	// link shaders to program
	_id = glCreateProgram();
	glAttachShader(_id, compute);
	glLinkProgram(_id); glCheckError();
	glGetProgramiv(_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(_id, 512, NULL, infoLog);
		std::cout << "ERROR: Shader program LINKING_FAILED\n" << infoLog << std::endl;
		return false;
	}

	//delete shaders
	glDeleteShader(compute); glCheckError();

	return true;
}

void ComputeShader::Dispatch(glm::ivec3 numGroups)
{
	glDispatchCompute(numGroups.x, numGroups.y, numGroups.z); glCheckError();
}
