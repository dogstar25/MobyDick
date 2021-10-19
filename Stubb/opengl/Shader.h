#pragma once

#include <string>
#include <glad/glad.h>
#include "BaseConstants.h"

struct ShaderSource
{
	std::string vertexSource;
	std::string fragmentSource;

};


class Shader
{
public:
	Shader(GLShaderType shaderType);
	~Shader();

	void bind();
	void unbind();
	void cleanup();

	GLuint vertexshaderId() { return m_vertextShaderId; }
	GLuint fragmentshaderId() { return m_fragmentShaderId; }
	

private:
	std::string m_fileName{};
	ShaderSource m_shaderSource{};
	unsigned int m_vertextShaderId{};
	unsigned int m_fragmentShaderId{};
	unsigned int m_shaderProgramId{};
	GLuint m_shaderId{};


	ShaderSource _parseShaderSource(const std::string& shaderFilename);
	GLint _compileShader(unsigned int type, const std::string shaderSource);
	int _getUniformLocation(std::string& uniformName);

};

