#pragma once
#include "GameObjectAnimation.h"

#include <map> 
#include <SDL.h>
#include <string>
#include <Box2D/Box2D.h>

//Forward declarations
class GameObjectDefinition;

using namespace std;

class GameObject
{
public:

	//Non Player Controlled methods
	void update();

	GameObjectDefinition* definition;

	string 	currentAnimationState;
	float angleAdjustment; //When rendering this object, adjust the angle by this degree. Added for reusing wall textures

	SDL_Texture* staticTexture; //Used if this is not an animated object
	//map<string, GameObjectAnimation> animations;
	b2Body* physicsBody;



};

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
	map<string, GameObjectAnimation*> animations;


};



