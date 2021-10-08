#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

struct Texture
{
	SDL_Texture* sdlTexture{};
	SDL_Surface* surface{};
	SDL_Rect textureAtlasQuad{};
	bool isRootTexture{ false };
};

#endif

