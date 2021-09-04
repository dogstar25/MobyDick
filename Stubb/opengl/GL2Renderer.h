#pragma once

#define GL_GLEXT_PROTOTYPES

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VertexBuffer.h"
#include "GLDrawer.h"


class GL2Renderer
{

public:
	static GL2Renderer& instance();

	void init(SDL_Window* window);

	//Note:the renderComponent will call this draw
	void drawSprite(int objectType, glm::vec2 position, GLint layer, GLfloat angle, GLfloat width, GLfloat height, glm::vec4 color, GLuint textureId, glm::vec2 textureCoords );
	const GLuint vao() { return m_vao; }
	const GLuint vbo() { return m_vbo; }
	const GLuint ibo() { return m_ibo; }
	void bind();

private:
	GL2Renderer() = default;
	~GL2Renderer();

	void _setVertexBufferAttriubuteLayout();
	void _buildDrawers();

	SDL_GLContext m_glcontext{};
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ibo;

	//GLDrawer m_glDrawers[static_cast<int>(GLDrawerType::count) + 1];

	GLDrawer m_spriteDrawer;
	GLDrawer m_lineDrawer;



	

	
};

