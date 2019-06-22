#pragma once
#include <SDL.h>
#include "Weapon.h"
class GameObject;
//class Weapon;

class PlayerObject : public GameObject
{
public:

	int direction, strafe;
	Weapon * weapon;
	b2Body* physicsBody;
	//direction 1 is forward, -1 is backward
	//strafe 1 is strafe left, -1 is strafe right

	PlayerObject();
	PlayerObject(string, int, int, int);
	~PlayerObject();

	void update();
	void render();
	b2Body* buildB2Body(GameObjectDefinition*);
	void handlePlayerMovementEvent(SDL_Event* event);
	void updatePlayerMovement();
	void addWeapon(string, float,float);

};

