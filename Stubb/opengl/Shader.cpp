#include "shader.h"

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& shaderName)
{

    GLint status{};

    //ToDo:Lookup the actual shader that you want
    if (shaderName == "BASIC") {

        m_shaderSource = _parseShaderSource("opengl/shaders/Basic.glsl");
    }
	if (shaderName == "UBER") {

		m_shaderSource = _parseShaderSource("opengl/shaders/uberShader.glsl");
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
