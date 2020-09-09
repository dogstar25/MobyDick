#ifndef RENDERER_GL_H
#define RENDERER_GL_H

#include <vector>
#include <map>
#include <optional>

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include <box2d/box2d.h>


class Renderer_GL
{
public:
	Renderer_GL();
	~Renderer_GL();

	void init(SDL_Window* window);
	static Renderer_GL& instance();
	void openGLSetup(SDL_Window* window);
	void LoadShaders();
	void testRender(SDL_Window* window);
	bool present();
	bool clear();

private:
	SDL_GLContext m_OpenGlContext;

	GLuint m_shaderProgram;

	GLuint m_VAO; // vertex array object
	GLuint m_VBO; // vertex buffer object

	GLuint m_verticesCount = 3;


};


#endif