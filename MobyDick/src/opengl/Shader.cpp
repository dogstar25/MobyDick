#include "shader.h"
#include "shaderCode.h"


#include <fstream>
#include <sstream>
#include <iostream>


Shader::Shader(GLShaderType shaderType)
{

    GLint status{};

    //ToDo:Lookup the actual shader that you want
    if (shaderType == GLShaderType::BASIC) {

        //m_shaderSource = _parseShaderSource("shaders/basic.glsl");
        m_shaderSource.vertexSource = std::string(basicShader::vertextShader);
        m_shaderSource.fragmentSource = std::string(basicShader::fragmentShader);
    }
    if (shaderType == GLShaderType::LINE) {

        //m_shaderSource = _parseShaderSource("shaders/basic.glsl");
        m_shaderSource.vertexSource = std::string(lineShader::vertextShader);
        m_shaderSource.fragmentSource = std::string(lineShader::fragmentShader);
    }
	if (shaderType == GLShaderType::UBER) {

		m_shaderSource = _parseShaderSource("../opengl/shaders/uberShader.glsl");
        //m_shaderSource.vertexSource = std::string(gonFrag1);
	}
	if (shaderType == GLShaderType::GLOW) {

		m_shaderSource.vertexSource = std::string(glowShader::vertextShader);
		m_shaderSource.fragmentSource = std::string(glowShader::fragmentShader);
	}

    m_vertextShaderId = glCreateShader(GL_VERTEX_SHADER);
    m_fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    //compile vertext shader
    status = _compileShader(m_vertextShaderId, m_shaderSource.vertexSource);
    if (status == GL_FALSE)
    {
        std::cout << "vertex shader compilation failed\n";
    }

    //compile fragment shader
    status = _compileShader(m_fragmentShaderId, m_shaderSource.fragmentSource);
    if (status == GL_FALSE)
    {
        std::cout << "fragment shader compilation failed\n";
    }

    //Greate a program for this shader
		//Create the shader program
    m_shaderProgramId = glCreateProgram();

	//Attach my built and ready shader program
    glAttachShader(m_shaderProgramId, m_vertextShaderId);
	glAttachShader(m_shaderProgramId, m_fragmentShaderId);

	//Link and use the program
	glLinkProgram(m_shaderProgramId);

    //cleanup
    //glDeleteShader(m_vertextShaderId);
    //glDeleteShader(m_fragmentShaderId);


}

Shader::~Shader()
{
}

void Shader::bind()
{
}

void Shader::unbind()
{
}

void Shader::cleanup()
{
    glDeleteShader(m_vertextShaderId);
    glDeleteShader(m_fragmentShaderId);

}

//ShaderSource Shader::_parseShaderSource2(const std::string& shaderSource)
//{
//
//
//
//
//}

ShaderSource Shader::_parseShaderSource(const std::string& shaderFilename)
{


    std::ifstream inputStream(shaderFilename);

    std::string line;
    std::stringstream ss[2];

    enum class shaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1};

    shaderType type{ shaderType::NONE };
    while(getline(inputStream, line)){

        if (line.find("#shader") != std::string::npos) {

            if (line.find("vertex") != std::string::npos) {
                type = shaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = shaderType::FRAGMENT;
            }

        }
        else {
            ss[(int)type] << line << "\n";
        }



    }

    inputStream.close();


    //ShaderSource* shaderSource = new ShaderSource();
    //shaderSource->fragmentSource = ss[(int)shaderType::FRAGMENT].str();
    //shaderSource->vertextSource = ss[(int)shaderType::VERTEX].str();

    ShaderSource shaderSource{ ss[(int)shaderType::VERTEX].str() , ss[(int)shaderType::FRAGMENT].str() };

    return shaderSource;
}

GLint Shader::_compileShader(unsigned int shaderId, const std::string shaderSource)
{
    GLint status{};

    const GLint len = strlen(shaderSource.c_str());
    const GLchar* src = shaderSource.c_str();

    glShaderSource(shaderId, 1, (const GLchar**)&src, &len);
    glCompileShader(shaderId);

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        std::cout << "Shader compilation failed\n";
    }

    return status;
}

int Shader::_getUniformLocation(std::string& uniformName)
{
    return 0;
}
