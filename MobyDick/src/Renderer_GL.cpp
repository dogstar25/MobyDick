#include "Renderer_GL.h"

#include <glm/glm.hpp>
#include <assert.h>

Renderer_GL::Renderer_GL()
{

}


Renderer_GL::~Renderer_GL()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	SDL_GL_DeleteContext(m_OpenGlContext);

}

Renderer_GL& Renderer_GL::instance()
{
	static Renderer_GL singletonInstance;
	return singletonInstance;

}


void Renderer_GL::init(SDL_Window* window)
{

	// Create OpenGL context
	m_OpenGlContext = SDL_GL_CreateContext(window);
	assert(m_OpenGlContext != NULL && "OpenGL failed to CreateContext");

	// Load OpenGL functions glad SDL
	gladLoadGLLoader(SDL_GL_GetProcAddress);

	// V-Sync
	SDL_GL_SetSwapInterval(1);


}

void Renderer_GL::openGLSetup(SDL_Window* window) // set up OpenGL
{
	GLint vpWidth, vpHeight;
	SDL_GL_GetDrawableSize(window, &vpWidth, &vpHeight);
	glViewport(0, 0, vpWidth, vpHeight);


	GLfloat vertices[] = {	 0.0f, -0.5f, 0.0f,
							-0.5f,  0.5f, 0.0f,
							 0.5f,  0.5f, 0.0f };

	LoadShaders();

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	
	// Bind the Vertex Array Object first, then bind and set Vertex Buffers and attribute pointers
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // ?
	glBindVertexArray(0);

}

void Renderer_GL::LoadShaders()
{
	// Shaders
	const char* vertexShaderSource = "#version 450 core\n"
		"layout (location = 0) in vec3 position;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
		"}\0";
	const char* fragmentShaderSource = "#version 450 core\n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";

	// Build compile VERTEX_SHADER
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Check for compile errors VERTEX_SHADER
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
		printf("Error: vertex shader compilation error: %s\n", infoLog);
	}

	// Build compile FRAGMENT_SHADER
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Check for compile errors FRAGMENT_SHADER
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
		printf("Error: fragment shader compilation error: %s\n", infoLog);
	}

	// Link shaders
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vertexShader);
	glAttachShader(m_shaderProgram, fragmentShader);
	// glBindFragDataLocation(shaderProgram, 0, "color");
	glLinkProgram(m_shaderProgram);

	// Check the linking errors
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_shaderProgram, sizeof(infoLog), NULL, infoLog);
		printf("Error: linking error: %s\n", infoLog);
	}

	// Link shaders with application
	GLuint position=0;
	glGetAttribLocation(m_shaderProgram, "position");
	glEnableVertexAttribArray(position);
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Delete shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Renderer_GL::testRender(SDL_Window* window)
{



	glClearColor(0.4f, 0.2f, 0.2f, 1.0f); // Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw functions
	glUseProgram(m_shaderProgram);
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, m_verticesCount);
	glBindVertexArray(0);

	SDL_GL_SwapWindow(window); // swap buffers


}