#pragma once
#include <map> 
#include <SDL.h>
#include <string>
#include <Box2D/Box2D.h>
#include "GameObjectAnimation.h"
#include "GameObjectDefinition.h"

using namespace std;
class Game;

class GameObject
{
public:

	//Non Player Controlled methods
	void update();

	GameObjectDefinition definition;
	int
		xDirection,
		yDirection,
		angle;
	string
		currentAnimationState;

	SDL_Texture* staticTexture; //Used if this is not an animated object
	map<string, GameObjectAnimation> animations;
	b2Body* physicsBody;



};

