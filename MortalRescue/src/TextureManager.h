#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <json/json.h>

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	bool init(SDL_Window*);
	bool draw();

private:

	SDL_Texture* playerTex;
	SDL_Rect srcRect, destRect;
	SDL_Renderer* pRenderer;

	
};

