#pragma once
#include "GameObjectAnimation.h"

#include <map> 
#include <SDL.h>
#include <string>
#include <Box2D/Box2D.h>

//Forward declarations
class GameObjectDefinition;
class Weapon;

using namespace std;

class GameObject
{
public:

	GameObject();
	~GameObject();

	//Non Player Controlled methods
	void update();
	void handlePlayerMovementEvent(SDL_Event* event);
	void updatePlayer();
	void updatePlayerMovement();
	void addWeapon(string, float, float);

	GameObjectDefinition* definition;

	int direction, strafe;
	Weapon * weapon;
	string 	currentAnimationState;
	float angleAdjustment; //When rendering this object, adjust the angle by this degree. Added for reusing wall textures

	SDL_Texture* staticTexture; //Used if this is not an animated object
	b2Body* physicsBody;



};

class GameObjectDefinition
{
public:

	GameObjectDefinition();
	~GameObjectDefinition();

	float
		xSize,
		ySize,
		initPosX,
		initPosY,
		speed,
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



