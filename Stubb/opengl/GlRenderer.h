#pragma once

#define GL_GLEXT_PROTOTYPES

#include <SDL2/SDL.h>
#include <glad/glad.h>



class GlRenderer
{

public:
	GlRenderer(SDL_Window* window);
	~GlRenderer();



private:

	SDL_GLContext m_glcontext{};
};

