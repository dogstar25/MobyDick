#pragma once
#include <SDL.h>

#include "Weapon.h"
#include "WorldObject.h"

class GameObject;
//class Weapon;

class PlayerObject : public WorldObject
{
public:

	int 
		direction, 
		strafe,
		pieceCollectedCount;

	shared_ptr<Weapon> weapon;

	PlayerObject();
	PlayerObject(string, int, int, int);
	~PlayerObject();

	void update();
	void render();
	void handlePlayerMovementEvent(SDL_Event* event);
	void updatePlayerMovement();
	void fire();
	void weaponLevelUp();
	void incrementPiecesCollected();
	void setBox2DUserData(PlayerObject*);

};

