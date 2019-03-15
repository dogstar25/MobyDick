#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <json/json.h>
#include <stdio.h>
#include <string>
#include <map>
#include <Box2D/Box2D.h>

#include "GameObject.h"
#include "SDL_ttf.h"

//Forward declarations
class GameObject;
class Texture;

using namespace std;

class TextureManager
{

public:

	TextureManager();
	~TextureManager();

	bool init(SDL_Window*);
	bool render(GameObject* gameObject);
	bool render(b2Body* body);
	bool present();
	bool clear();
	const Texture* getTexture(string id);
	TTF_Font* getFont(string id);

	void drawPoly(b2Body* body);
	void drawPoints(SDL_Point *);
	void drawLine(b2Vec2, b2Vec2);

private:

	SDL_Renderer* pRenderer;
	map<string, unique_ptr<Texture>> textureMap;
	map<string, TTF_Font*> fontMap;
	bool loadTextures();
	
};

class Texture
{
public:

	SDL_Texture* sdlTexture;
	SDL_Surface* surface;


};

