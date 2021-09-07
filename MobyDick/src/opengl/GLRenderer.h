#pragma once

#define GL_GLEXT_PROTOTYPES

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GLDrawer.h"
#include "DrawBatch.h"
#include "../Renderer.h"



class GLRenderer : public Renderer
{

public:
	GLRenderer();
	~GLRenderer();

	SDL_Renderer* sdlRenderer() { return nullptr; }
	void init(SDL_Window* window);
	bool present() { return true; };
	bool clear() { return true; };
	SDL_Texture* createTextureFromSurface(SDL_Surface* surface) { return nullptr; };
	//Note:the renderComponent will call this draw
	void drawSprite(int objectType, glm::vec2 position, GLint layer, GLfloat angle, GLfloat width, GLfloat height, glm::vec4 color, GLuint textureId, glm::vec2 textureCoords );
	void drawSprite(SDL_FRect quad, SDL_Color color, SDL_Texture* texture, 
		SDL_Rect* textureSrcQuad, float angle, bool outline, SDL_Color outlineColor) override;
	void drawLine(b2Vec2 lineStart, b2Vec2 lineEnd, SDL_Color color) {};
	void drawQuad(SDL_FRect quad, SDL_Color color, bool outline, SDL_Color outlineColor) {};

	void bind();

	std::shared_ptr<GLDrawer> spriteDrawer(){ return m_spriteDrawer; }

private:

	void _setVertexBufferAttriubuteLayout();
	void _buildBatchDrawers();

	SDL_GLContext m_glcontext{};

	//GLDrawer m_glDrawers[static_cast<int>(GLDrawerType::count) + 1];

	std::shared_ptr<GLDrawer> m_spriteDrawer;
	std::shared_ptr<GLDrawer> m_lineDrawer;

	//Projection matrix
	glm::mat4  m_projectionMatrix{1.0};

	//array[objecttypes][textures][shaders]
	DrawBatch m_drawBatches[MAX_OBJECT_TYPES][MAX_GL_TEXTURES][MAX_GL_SHADER_TYPES];
	

	

	
};

