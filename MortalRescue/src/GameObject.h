#pragma once
#include "GameObjectAnimation.h"
#include "TextureManager.h"

#include <map> 
#include <SDL.h>
#include <string>
#include <Box2D/Box2D.h>

//Forward declarations
class GameObjectDefinition;
class Texture;

extern Game* game;

using namespace std;

/*
Game Object Types
*/
enum GameObjectType {

	BASE_OBJECT = 0,
	WORLD_OBJECT = 1,    //majority of game objects - any physics object
	PLAYER_OBJECT = 2,
	UI_OBJECT = 3

};
enum GameObjectShape {

	RECTANGLE = 0,
	CIRCLE = 1

};

static const float DEGTORAD = 0.0174532925199432957f;

class GameObject
{
public:

	GameObject();
	GameObject(string,int,int,int);
	virtual ~GameObject();

	virtual void update();
	virtual void render();

	GameObjectDefinition* definition;

	float xPos, yPos, angle;
	string 	currentAnimationState;

	Texture* texture;

};

class GameObjectDefinition
{
public:

	GameObjectDefinition();
	~GameObjectDefinition();

	float
		xSize,
		ySize,
		speed,
		friction,
		density,
		linearDamping,
		collisionRadius,
		angularDamping;
	string
		id,
		description,
		physicsType,
		collisionShape,
		texture;
	bool
		isAnimated,
		isPhysicsObject,
		isPrimitiveShape,
		isPlayerObject,
		isDynamicText;

	SDL_Color primativeColor; //If object is a primative shape, what color is it
	map<string, GameObjectAnimation*> animations;


};



