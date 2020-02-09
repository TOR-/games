#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <string>
#include <SDL2/SDL.h>

#include "Shader.h"

Shader::Shader() {}
Shader::~Shader() {}

bool Shader::CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader)
{
	std::ifstream shaderFile(fileName);
	if(shaderFile.is_open())
	{
		std::stringstream ss;
		ss << shaderFile.rdbuf();
		std::string s = ss.str();
		const char *source = s.c_str();

		outShader = glCreateShader(shaderType);
		glShaderSource(outShader, 1, &(source), nullptr);
		glCompileShader(outShader);
		if(!IsCompiled(outShader))
		{
			SDL_Log("Failed to compile shader file %s.", fileName.c_str());
			return false;
		}
	}
	else
	{
		SDL_Log("Failed to open shader file %s.", fileName.c_str());
		return false;
	}
	return true;
}

bool Shader::Load(const std::string& vtxName, const std::string& fragName)
{
	if(!CompileShader(vtxName, GL_VERTEX_SHADER, _vtxShaderID) ||
			!CompileShader(fragName, GL_FRAGMENT_SHADER, _fragShaderID))
		return false;

	_shaderProgramID = glCreateProgram();
	glAttachShader(_shaderProgramID, _vtxShaderID);
	glAttachShader(_shaderProgramID, _fragShaderID);
	glLinkProgram(_shaderProgramID);

	if(!IsValidProgram())
		return false;
	return true;
}

void Shader::Unload()
{
	glDeleteProgram(_shaderProgramID);
	glDeleteShader(_vtxShaderID);
	glDeleteShader(_fragShaderID);
}

bool Shader::IsCompiled(GLuint shader)
{
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status != GL_TRUE)
	{
		const int infoLogBufSize = 512;
		char buf[infoLogBufSize];
		memset(buf, 0, infoLogBufSize);
		glGetShaderInfoLog(shader, infoLogBufSize-1, nullptr, buf);
		SDL_Log("%s:%d:GLSL Compile Failed:\n\t%s.",__FILE__, __LINE__, buf);
		return false;
	}
	return true;
}

bool Shader::IsValidProgram()
{
	GLint status;
	glGetProgramiv(_shaderProgramID, GL_LINK_STATUS, &status);
	if(status != GL_TRUE)
	{
		const int infoLogBufSize = 512;
		char buf[infoLogBufSize];
		memset(buf, 0, infoLogBufSize);
		glGetProgramInfoLog(_shaderProgramID, infoLogBufSize-1, nullptr, buf);
		SDL_Log("%s:%d:GLSL Compile Failed:\n\t%s.", __FILE__, __LINE__, buf);
		return false;
	}
	return true;
}

void Shader::SetActive()
{
	glUseProgram(_shaderProgramID);
}
