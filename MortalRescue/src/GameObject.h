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

static const short CHILD_POSITIONS = 9;

class GameObject
{
public:

	enum GameObjectMouseState {

		MOUSE_NONE = 0,
		MOUSE_HOVER = 1,
		MOUSE_HOLD = 2,
		MOUSE_CLICKED = 3

	};

	GameObject();
	GameObject(string,float,float,float);
	virtual ~GameObject();

	virtual void update();
	virtual void render();

	virtual SDL_Rect  getPositionRect();
	virtual SDL_Rect  getRenderDestRect();
	virtual SDL_Texture* getRenderTexture(SDL_Texture *);
	virtual SDL_Rect* getRenderTextureRect(SDL_Rect*);
	virtual void setPosition(b2Vec2, float);
	virtual void addWeapon(string, float, float);
	virtual void onMouseHoverRender();
	virtual void onMouseClickRender();
	virtual void onMouseHoldRender();
	virtual void onMouseClickEvent();
	virtual void setActive(bool);
	void updateMouseState();

	void renderChildObjects();
	void updateChildObjects();
	b2Vec2 calcChildPosition(shared_ptr<GameObject>, int, int, int);
	b2Vec2 matchParentRotation(SDL_Rect, SDL_Rect, float);
	void buildChildren();
	
	

	//Object Attrbutes
	string 
		id,
		definitionId,
		currentAnimationState;
	bool
		isAnimated,
		removeFromWorld;

	SDL_Color 
		color;
	float 
		xPos,
		yPos,
		xSize,
		ySize,
		test,
		angle;

	int
		mouseState;

	Texture* texture;
	//pointer to the definition
	GameObjectDefinition* definition;

	//If animated, will contain all animations
	map<string, Animation*> animations;

	//Child Object collection
	array <vector<shared_ptr<GameObject>>, CHILD_POSITIONS> childObjects;
	
};



