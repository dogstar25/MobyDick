#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <json/json.h>
#include <stdio.h>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include "GameObject.h"


using namespace std;

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	bool init(SDL_Window*);
	bool render(GameObject* gameObject);
	SDL_Texture* getTexture(string id);

private:

	SDL_Renderer* pRenderer;
	map<string, SDL_Texture*> textureMap;

	bool loadTextures();
	
};

