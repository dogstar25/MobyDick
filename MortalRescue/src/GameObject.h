#pragma once
#include <map> 
#include <SDL.h>
#include <string>
#include <Box2D/Box2D.h>
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

	float
		xPos,
		yPos,
		xSize,
		ySize,
		initPosX,
		initPosY,
		playerSpeed,
		friction,
		density;
	int 
		xDirection, 
		yDirection;
	string 
		id, 
		description;
	bool 
		isStaticObject, 
		isAnimated, 
		isPhysicsObject,
		isPrimitiveShape;

	SDL_Texture* staticTexture; //Used if this is not an animated object
	SDL_Color primativeColor; //If object is a primative shape, what color is it
	map<string, GameObjectAnimation> animations;
	b2Body* physicsBody;



};

