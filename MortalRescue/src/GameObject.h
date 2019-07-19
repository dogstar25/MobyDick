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

	//Object Attrbutes
	string 
		id,
		definitionId,
		currentAnimationState;
	bool
		isAnimated,
		removeFromWorld; // when this is true the removal loop should remove it from the main game world collection
	SDL_Color 
		color;
	float 
		xPos,
		yPos,
		xSize,
		ySize,
		angle;

	Texture* texture;
	//pointer to the definition
	GameObjectDefinition* definition;

	map<string, GameObjectAnimation*> animations;

};



