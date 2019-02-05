#pragma once
#include "GameObject.h"
#include "game.h"


class PlayerObject :public GameObject
{
public:

	int direction, strafe;
	//direction 1 is forward, -1 is backward
	//strafe 1 is strafe left, -1 is strafe right

	void handlePlayerMovementEvent(SDL_Event* event);
	void updatePlayer();
	void updatePlayerMovement();

};

