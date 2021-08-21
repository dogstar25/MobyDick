#include "glRenderer.h"

#include <SDL2/SDL_opengl.h>

#include <memory>
#include <stdio.h>
#include <iostream>

#include "../opengl/Shader.h"
#include "../opengl/VertexBuffer.h"
#include "../opengl/IndexBuffer.h"


GlRenderer::GlRenderer(SDL_Window* window)
{
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


    SDL_GLContext m_glcontext = SDL_GL_CreateContext(window);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

}

GlRenderer::~GlRenderer()
{

    SDL_GL_DeleteContext(m_glcontext);

}