#pragma once

#include <GL/gl.h>
#include <string>

class Shader
{
	public:
		Shader();
		~Shader();
		bool Load(const std::string& vtxName, const std::string& fragName);
		void SetActive();
	private:
		bool CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);
		bool IsCompiled(GLuint shader);
		bool IsValidProgram();
		void Unload();

		GLuint _vtxShaderID;
		GLuint _fragShaderID;
		GLuint _shaderProgramID;
};
