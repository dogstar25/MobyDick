#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <json/json.h>
#include <stdio.h>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <Box2D/Box2D.h>
#include "GameObject.h"



using namespace std;

class TextureManager
{
public:
	bool init(SDL_Window*);
	bool render(GameObject* gameObject);
	bool render(b2Body* body);
	bool present();
	bool clear();
	SDL_Texture* getTexture(string id);
	void drawPoly(b2Body* body);

private:

	SDL_Renderer* pRenderer;
	map<string, SDL_Texture*> textureMap;

	bool loadTextures();
	
};

