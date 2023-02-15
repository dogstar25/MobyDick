#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "opengl/GLRenderer.h"

class Texture
{
public:
	SDL_Texture* sdlTexture{};
	SDL_Surface* surface{};
	SDL_Rect textureAtlasQuad{};
	bool isRootTexture{ false };
	bool applyFlip{ false };
	GL_TextureIndexType openglTextureIndex{};
};

class AtlasTexture : public Texture
{
public:
	SDL_Texture* sdlTexture{};
	SDL_Surface* surface{};
};

#endif

