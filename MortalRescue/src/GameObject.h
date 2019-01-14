#pragma once
#include <map> 
#include <SDL.h>
#include <string>
#include <Box2D/Box2D.h>
#include "GameObjectAnimation.h"
#include "GameObjectDefinition.h"

using namespace std;

class GameObject
{
public:
	//Player controller specific methods
	void handlePlayerMovementEvent(SDL_Event* event);
	void updatePlayer();

	//Non Player Controlled methods
	void update();

	GameObjectDefinition definition;
	float
		xPos,
		yPos;
	int
		xDirection,
		yDirection;
	string
		currentAnimationState;

	SDL_Texture* staticTexture; //Used if this is not an animated object
	map<string, GameObjectAnimation> animations;
	b2Body* physicsBody;



};

