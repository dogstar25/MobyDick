#include "TextureManager.h"


bool TextureManager::init(SDL_Window* pWindow)
{

	pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
	SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);

	SDL_Surface* tempSurface = IMG_Load("assets/player.png");
	playerTex = SDL_CreateTextureFromSurface(pRenderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return true;
}

bool TextureManager::draw()
{
	SDL_RenderClear(pRenderer);
	SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
	destRect.h = 64;
	destRect.w = 64;

	SDL_RenderCopy(pRenderer, playerTex, NULL, &destRect);
	SDL_RenderPresent(pRenderer);

	return true;
}

TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
	SDL_DestroyRenderer(pRenderer);
	
}
