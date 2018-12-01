#pragma once
#include <map> 
#include <SDL.h>
#include <string>
#include "GameObjectAnimation.h"

using namespace std;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void handleEvent(SDL_Event* event);
	void update();
	void init();

	float xPos, yPos, xSize, ySize, speed, xVelocity, yVelocity;
	string id, description;
	bool isStaticObject, isAnimated;
	SDL_Texture* staticTexture; //Used if this is not an animated object
	map<string, GameObjectAnimation> animations;



};

