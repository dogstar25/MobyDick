#ifndef PLAYER_OBJECT_H
#define PLAYER_OBJECT_H

#include <SDL2/SDL.h>

#include "WorldObject.h"


class PlayerObject : public WorldObject
{
public:

	int 
		direction, 
		strafe,
		pieceCollectedCount;

	PlayerObject();
	PlayerObject(std::string, int, int, int);
	~PlayerObject();

	void update();
	void render();
	void handlePlayerMovementEvent();
	void updatePlayerMovement();
	void fire();
	void weaponLevelUp();
	void incrementPiecesCollected();
	void setBox2DUserData(PlayerObject*);

};

#endif