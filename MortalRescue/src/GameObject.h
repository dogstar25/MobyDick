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

/*
Game Object Types
*/
enum GameObjectType {

	PRIMITIVE_OBJECT = 0,  //p
	WORLD_OBJECT = 1,    //majority of game objects - any physics object
	PLAYER_OBJECT = 2,
	UI_OBJECT = 3

};


class GameObject
{
public:

	GameObject();
	virtual ~GameObject();

	//Non Player Controlled methods
	void update();
	void render();

	GameObjectDefinition* definition;

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



