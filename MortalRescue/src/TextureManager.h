#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <json/json.h>
#include <stdio.h>
#include <string>
#include <map>
#include <Box2D/Box2D.h>
#include "GameObject.h"

//Forward declarations
class GameObject;
class Texture;

using namespace std;

class TextureManager
{
public:
	bool init(SDL_Window*);
	bool render(GameObject* gameObject);
	bool render(b2Body* body);
	bool present();
	bool clear();
	Texture* getTexture(string id);
	void drawPoly(b2Body* body);
	void drawPoints(SDL_Point *);
	void drawLine(b2Vec2, b2Vec2);

private:

	SDL_Renderer* pRenderer;
	map<string, Texture*> textureMap;

	bool loadTextures();
	
};

class Texture
{
public:

	SDL_Texture* texture;
	SDL_Surface* surface;

};

