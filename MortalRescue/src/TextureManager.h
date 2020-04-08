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
class PlayerObject;
class WorldObject;
class TextObject;
class ParticleObject;
class Game;

using namespace std;

extern Game* game;

class TextureManager
{

public:

	/*
	This is a singleton class using a local staic variable returned as a reference
	*/
	static TextureManager& instance();
	bool init(SDL_Window*);
	void render(SDL_Texture* texture, SDL_Color color, SDL_Rect* srcRect, SDL_Rect* destRect, float angle);
	void render(SDL_Rect* destRect, SDL_Color color);
	bool present();
	bool clear();
	Texture* getTexture(string id);
	string getFont(string id);
	void drawPoly(b2Body* body);
	void drawPoints(SDL_Point*);
	void drawLine(b2Vec2, b2Vec2);
	void drawGlowLine(b2Vec2, b2Vec2, SDL_Color);
	void drawGlowLine2(b2Vec2, b2Vec2, SDL_Color);
	void outLineObject(GameObject* gameObject, float lineSize);
	void outLineObject(WorldObject* gameObject);
	SDL_Texture* createTextureFromSurface(SDL_Surface* surface);

private:
	TextureManager();
	~TextureManager();

	SDL_Renderer* m_Renderer;
	map<string, unique_ptr<Texture>> m_textureMap;
	map<string, string> m_fontMap;
	bool loadTextures();


	

};


