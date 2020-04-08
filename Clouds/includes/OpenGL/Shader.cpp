#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "OpenGLDebug.h"

Shader::Shader()
{
	_id = 0;
}

Shader::~Shader()
{
	// ShaderBase cleans up
}


Shader::Shader(const char* vertexPath, const char* fragPath, const char* geoPath, bool debug)
{
	std::ifstream vertexFile;
	std::ifstream fragFile;
	std::ifstream geoFile;
	std::string vertexCode;
	std::string fragCode;
	std::string geoCode;

	//read vertex file
	vertexFile.open(vertexPath);
	if (vertexFile.is_open())
	{
		std::stringstream vertexStream;
		vertexStream << vertexFile.rdbuf();
		vertexCode = vertexStream.str();
	}
	else { std::cout << "ERROR: Can't open vertex shader file at: " << vertexPath << std::endl; }
	vertexFile.close();

	//read fragment file
	fragFile.open(fragPath);
	if (fragFile.is_open())
	{
		std::stringstream fragStream;
		fragStream << fragFile.rdbuf();
		fragCode = fragStream.str();
	}
	else { std::cout << "ERROR: Can't open fragment shader file at: " << fragPath << std::endl; }
	fragFile.close();

	if (geoPath != nullptr)
	{
		//read geometry file
		geoFile.open(geoPath);
		if (geoFile.is_open())
		{
			std::stringstream geoStream;
			geoStream << geoFile.rdbuf();
			geoCode = geoStream.str();
		}
		else { std::cout << "ERROR: Can't open geometry shader file at: " << geoPath << std::endl; }
		geoFile.close();
	}
	//cast string into array of GLchar
	const GLchar* vCode = vertexCode.c_str();
	const GLchar* fCode = fragCode.c_str();
	const GLchar* gCode = geoCode.c_str();

	if (debug)
	{
		std::cout << vCode << std::endl << std::endl;
		std::cout << fCode << std::endl << std::endl;
		if (geoPath != nullptr)std::cout << fCode << std::endl << std::endl;
	}

	GLuint vertex, fragment, geometry;
	GLint success;
	GLchar infoLog[512];


	//compile vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vCode, NULL);
	glCompileShader(vertex); glCheckError();
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR: Vertex shader COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "Compiled Vertex Shader: " << vertexPath << std::endl;
	}
	//compile fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fCode, NULL);
	glCompileShader(fragment); glCheckError();
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR: Fragment shader COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "Compiled Fragment Shader: " << fragPath << std::endl;
	}
	if (geoPath != nullptr) {
		//compile geometry shader
		geometry = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(geometry, 1, &gCode, NULL);
		glCompileShader(geometry); glCheckError();
		glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(geometry, 512, NULL, infoLog);
			std::cout << "ERROR: Geometry shader COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		else
		{
			std::cout << "Compiled Geometry Shader: " << geoPath << std::endl;
		}
	}
	// link shaders to program
	_id = glCreateProgram();
	glAttachShader(_id, vertex); glCheckError();
	glAttachShader(_id, fragment); glCheckError();
	if (geoPath != nullptr) { glAttachShader(_id, geometry); glCheckError(); }
	glLinkProgram(_id); glCheckError();
	glGetProgramiv(_id, GL_LINK_STATUS, &success); glCheckError();
	if (!success)
	{
		glGetProgramInfoLog(_id, 512, NULL, infoLog);
		std::cout << "ERROR: Shader program LINKING_FAILED\n" << infoLog << std::endl;
	}

	//delete shaders
	glDeleteShader(vertex); glCheckError();
	glDeleteShader(fragment); glCheckError();
	if (geoPath != nullptr) { glDeleteShader(geometry); glCheckError(); }
}


bool Shader::CompileFromCode(const char* vertexCode, const char* fragCode, const char* geoCode)
{
	if (_id)
	{
		glDeleteProgram(_id); glCheckError();
	}

	GLuint vertex, fragment, geometry;
	GLint success;
	GLchar infoLog[512];

	//compile vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexCode, NULL);
	glCompileShader(vertex); glCheckError();
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR: Vertex shader COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}

	//compile fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragCode, NULL);
	glCompileShader(fragment); glCheckError();
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR: Fragment shader COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}

	if (geoCode != nullptr) {
		//compile geometry shader
		geometry = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(geometry, 1, &geoCode, NULL);
		glCompileShader(geometry); glCheckError();
		glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(geometry, 512, NULL, infoLog);
			std::cout << "ERROR: Geometry shader COMPILATION_FAILED\n" << infoLog << std::endl;
			return false;
		}
	}
	// link shaders to program
	_id = glCreateProgram();
	glAttachShader(_id, vertex); glCheckError();
	glAttachShader(_id, fragment); glCheckError();
	if (geoCode != nullptr) { glAttachShader(_id, geometry); glCheckError(); }
	glLinkProgram(_id); glCheckError();
	glGetProgramiv(_id, GL_LINK_STATUS, &success); glCheckError();
	if (!success)
	{
		glGetProgramInfoLog(_id, 512, NULL, infoLog);
		std::cout << "ERROR: Shader program LINKING_FAILED\n" << infoLog << std::endl;
		return false;
	}

	//delete shaders
	glDeleteShader(vertex); glCheckError();
	glDeleteShader(fragment); glCheckError();
	if (geoCode != nullptr) { glDeleteShader(geometry); glCheckError(); }

	return true;
}

bool Shader::CompileFromPath(const char* vertexPath, const char* fragPath, const char* geoPath)
{
	if (_id)
	{
		glDeleteProgram(_id); glCheckError();
	}

	std::ifstream vertexFile;
	std::ifstream fragFile;
	std::ifstream geoFile;
	std::string vertexCode;
	std::string fragCode;
	std::string geoCode;

	//read vertex file
	vertexFile.open(vertexPath);
	if (vertexFile.is_open())
	{
		std::stringstream vertexStream;
		vertexStream << vertexFile.rdbuf();
		vertexCode = vertexStream.str();
	}
	else
	{
		std::cout << "ERROR: Can't open vertex shader file at: " << vertexPath << std::endl;
		return false;
	}
	vertexFile.close();

	//read fragment file
	fragFile.open(fragPath);
	if (fragFile.is_open())
	{
		std::stringstream fragStream;
		fragStream << fragFile.rdbuf();
		fragCode = fragStream.str();
	}
	else
	{
		std::cout << "ERROR: Can't open fragment shader file at: " << fragPath << std::endl;
		return false;
	}
	fragFile.close();

	if (geoPath != nullptr)
	{
		//read geometry file
		geoFile.open(geoPath);
		if (geoFile.is_open())
		{
			std::stringstream geoStream;
			geoStream << geoFile.rdbuf();
			geoCode = geoStream.str();
		}
		else
		{
			std::cout << "ERROR: Can't open geometry shader file at: " << geoPath << std::endl;
			return false;
		}
		geoFile.close();
	}
	//cast string into array of GLchar
	const GLchar* vCode = vertexCode.c_str();
	const GLchar* fCode = fragCode.c_str();
	const GLchar* gCode = geoCode.c_str();

	GLuint vertex, fragment, geometry;
	GLint success;
	GLchar infoLog[512];


	//compile vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vCode, NULL);
	glCompileShader(vertex); glCheckError();
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR: Vertex shader COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}

	//compile fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fCode, NULL);
	glCompileShader(fragment); glCheckError();
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR: Fragment shader COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}

	if (geoPath != nullptr) {
		//compile geometry shader
		geometry = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(geometry, 1, &gCode, NULL);
		glCompileShader(geometry); glCheckError();
		glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(geometry, 512, NULL, infoLog);
			std::cout << "ERROR: Geometry shader COMPILATION_FAILED\n" << infoLog << std::endl;
			return false;
		}
	}

	// link shaders to program
	_id = glCreateProgram();
	glAttachShader(_id, vertex); glCheckError();
	glAttachShader(_id, fragment); glCheckError();
	if (geoPath != nullptr) { glAttachShader(_id, geometry); glCheckError(); }
	glLinkProgram(_id); glCheckError();
	glGetProgramiv(_id, GL_LINK_STATUS, &success); glCheckError();
	if (!success)
	{
		glGetProgramInfoLog(_id, 512, NULL, infoLog);
		std::cout << "ERROR: Shader program LINKING_FAILED\n" << infoLog << std::endl;
		return false;
	}

	//delete shaders
	glDeleteShader(vertex); glCheckError();
	glDeleteShader(fragment); glCheckError();
	if (geoPath != nullptr) { glDeleteShader(geometry); glCheckError(); }

	return true;
}