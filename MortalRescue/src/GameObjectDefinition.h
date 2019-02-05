#pragma once
#include <map> 
#include <SDL.h>
#include <string>
#include <Box2D/Box2D.h>
#include "GameObjectAnimation.h"

class GameObjectDefinition
{
public:
	
	float
		xSize,
		ySize,
		initPosX,
		initPosY,
		playerSpeed,
		friction,
		density,
		linearDamping,
		angularDamping;
	int
		xDirection,
		yDirection;
	string
		id,
		description,
		physicsType,
		texture;
	bool
		isAnimated,
		isPhysicsObject,
		isPrimitiveShape,
		isPlayerObject;

	SDL_Color primativeColor; //If object is a primative shape, what color is it
	map<string, GameObjectAnimation> animations;


};

