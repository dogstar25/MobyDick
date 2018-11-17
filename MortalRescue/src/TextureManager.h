#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <json/json.h>
#include <fstream>
#include <stdio.h>
#include <string>
#include <map>
using namespace std;


class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	bool init(SDL_Window*);
	bool draw();

private:

	SDL_Rect srcRect, destRect;
	SDL_Renderer* pRenderer;
	map<string, SDL_Texture*> textureMap;

	bool loadTextures();
	
};

