#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glad/glad.h>

class Texture
{
public:
	SDL_Texture* sdlTexture{};
	SDL_Surface* surface{};
	SDL_Rect textureAtlasQuad{};
	bool isRootTexture{ false };
	GLuint gLTextureId{};
};

class AtlasTexture : public Texture
{
public:
	SDL_Texture* sdlTexture{};
	SDL_Surface* surface{};
};

#endif

