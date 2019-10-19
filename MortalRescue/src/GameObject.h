#pragma once
#include "Animation.h"
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
enum EntityCategory {
	BOUNDARY = 0x0001,
	PARTICLE_BULLET = 0x0002
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

	virtual SDL_Rect*  getRenderDestRect(SDL_Rect*);
	virtual SDL_Texture* getRenderTexture(SDL_Texture *);
	virtual SDL_Rect* getRenderTextureRect(SDL_Rect*);

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

	//If animated, will contain all animations
	map<string, Animation*> animations;

};



