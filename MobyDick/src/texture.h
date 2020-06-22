#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>

struct Texture
{

	SDL_Texture* sdlTexture;
	SDL_Surface* surface;

};

#endif

