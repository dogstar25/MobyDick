#pragma once
#include <SDL.h>
#include "Weapon.h"
class GameObject;
//class Weapon;

class PlayerObject :public GameObject
{
public:

	int direction, strafe;
	Weapon * weapon;
	//direction 1 is forward, -1 is backward
	//strafe 1 is strafe left, -1 is strafe right

	PlayerObject();
	~PlayerObject();

	void handlePlayerMovementEvent(SDL_Event* event);
	void updatePlayer();
	void updatePlayerMovement();
	void addWeapon(string, float,float);

};

