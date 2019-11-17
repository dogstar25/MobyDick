#pragma once
#include <array>

#include <map> 
#include <SDL.h>
#include <string>
#include <Box2D/Box2D.h>

//#include "Animation.h"
#include "TextureManager.h"


//Forward declarations
class GameObjectDefinition;
class Texture;
class TextObject;
class Animation;

using namespace std;

/*
Game Object Types
*/
enum GameObjectType {

	GAME_OBJECT = 0,
	WORLD_OBJECT = 1,    //majority of game objects - any physics object
	PLAYER_OBJECT = 2,
	TEXT_OBJECT = 3

};
enum EntityCategory {
	BOUNDARY = 0x0001,
	PARTICLE_BULLET = 0x0002
};

enum GameObjectShape {

	RECTANGLE = 0,
	CIRCLE = 1

};

enum ChildObjectPosition {

	TOP_LEFT = 1,
	TOP = 2,
	TOP_RIGHT = 3,
	LEFT = 4,
	CENTER = 5,
	RIGHT = 6,
	BOTTOM_LEFT = 7,
	BOTTOM = 8,
	BOTTOM_RIGHT = 9,

};

static const float DEGTORAD = 0.0174532925199432957f;
static const short CHILD_POSITIONS = 9;

class GameObject
{
public:

	GameObject();
	GameObject(string,int,int,int);
	virtual ~GameObject();

	virtual void update();
	virtual void render();

	virtual SDL_Rect  getRenderDestRect();
	virtual SDL_Texture* getRenderTexture(SDL_Texture *);
	virtual SDL_Rect* getRenderTextureRect(SDL_Rect*);
	virtual void setPosition(b2Vec2, float);

	void addChildObject(GameObject*, short);
	virtual void addWeapon(string, float, float);
	void renderChildObjects();
	void updateChildObjects();
	b2Vec2 calcChildPosition(b2Vec2, short, bool,SDL_Rect);
	void buildChildren();
	

	//Object Attrbutes
	string 
		id,
		definitionId,
		currentAnimationState;
	bool
		isAnimated,
		removeFromWorld,
		isChildObject;

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

	//Child Object collection
	array <vector<shared_ptr<GameObject>>, CHILD_POSITIONS> childObjects;
	//array <vector<GameObject*>, CHILD_POSITIONS> childObjects;
	
};



