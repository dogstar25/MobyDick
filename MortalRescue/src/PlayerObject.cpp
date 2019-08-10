#include "game.h"
#include "PlayerObject.h"
#include "WorldObject.h"
#include "Weapon.h"
#include "GameObjectDefinition.h";

#include <math.h>

PlayerObject::PlayerObject()
{
}

PlayerObject::PlayerObject(string gameObjectId, int xMapPos, int yMapPos, int angleAdjust) :
	WorldObject(gameObjectId, xMapPos, yMapPos, angleAdjust)
{

	this->direction = 0;
	this->strafe = 0;
	this->currentAnimationState = "IDLE";


}

PlayerObject::~PlayerObject()
{

	//int todd = 1;
	delete this->weapon;

}

void PlayerObject::handlePlayerMovementEvent(SDL_Event* event)
{

	int count;
	int* mouseXChg = NULL, *mouseYChg = NULL;
	//const Uint8 *state;


	//SDL_PumpEvents();
	const Uint8 *state = SDL_GetKeyboardState(&count);
	this->direction = 0;
	this->strafe = 0;

	if (state[SDL_SCANCODE_W])
	{
		this->direction = -1;
	}
	if (state[SDL_SCANCODE_S])
	{
		this->direction = 1;
	}
	if (state[SDL_SCANCODE_A])
	{
		this->strafe = 1;
	}
	if (state[SDL_SCANCODE_D])
	{
		this->strafe = -1;
	}

	/*
		if (event->type == SDL_KEYUP) {

			switch (event->key.keysym.sym) {
			case SDLK_w:
				//this->yDirection = 0;
				this->direction = 0;
				break;
			case SDLK_s:
				this->direction = 0;
				break;
			case SDLK_a:
				this->strafe = 0;
				break;
			case SDLK_d:
				this->strafe = 0;
				break;
			}
		}

		if (event->type == SDL_KEYDOWN) {
			switch (event->key.keysym.sym) {
			case SDLK_w:
				//this->yDirection = -1;
				this->direction = -1;
				break;
			case SDLK_s:
				//this->yDirection = 1;
				this->direction = 1;
				break;
			case SDLK_a:
				//this->xDirection = -1;
				this->strafe = 1;
				break;
			case SDLK_d:
				//this->xDirection = 1;
				this->strafe = -1;
				break;
			}
		}
	*/

	//method 1
	float angularVelocity = event->motion.xrel * game->config.mouseSensitivity;
	this->physicsBody->SetAngularVelocity(angularVelocity);

	//method 2
	//float desiredAngle = atan2f(-event->motion.x, event->motion.y);
	//this->physicsBody->SetTransform(this->physicsBody->GetPosition(), desiredAngle);

}


void PlayerObject::update()
{
	//Call base game object update
	GameObject::update();

	//Call all player movement updates
	this->updatePlayerMovement();

}

void PlayerObject::render()
{
	WorldObject::render();
}


void PlayerObject::updatePlayerMovement()
{
	/*
	Lots of tweeks can be down here depending on the game you are building for smoother movement
	*/

	float acceleration = this->speed;
	float velocity = acceleration;

	//Calc direction XY
	//float dx = cos(this->physicsBody->GetAngle()) * velocity * this->direction; // X-component.
	//float dy = sin(this->physicsBody->GetAngle()) * velocity * this->direction; // Y-component.
	float dx = cos(1.5708) * velocity * this->direction; // X-component.
	float dy = sin(1.5708) * velocity * this->direction; // Y-component.

	//calc strafe xy and add direction and strafe vectors
	//1.5708 is 90 degrees
	//float sx = cos(this->physicsBody->GetAngle() + 1.5708) * velocity * this->strafe; // X-component.
	//float sy = sin(this->physicsBody->GetAngle() + 1.5708) * velocity * this->strafe; // Y-component.
	float sx = cos((1.5708) + 1.5708) * velocity * this->strafe; // X-component.
	float sy = sin((1.5708) + 1.5708) * velocity * this->strafe; // Y-component.

	//Create the vector for forward/backward  direction
	b2Vec2 directionVector = b2Vec2(dx, dy);

	//Create the vector for strafe direction
	b2Vec2 strafeVector = b2Vec2(sx, sy);

	//Initialize new final movemtn vector
	b2Vec2 vec2;
	vec2.SetZero();

	vec2 = (directionVector + strafeVector);

	//Update Animation state
	if (vec2.Length() > 0)
	{
		this->currentAnimationState = "RUN";
	}
	else
	{
		this->currentAnimationState = "IDLE";
	}

	//this->physicsBody->SetTransform(vec3, this->physicsBody->GetAngle());
	this->physicsBody->SetLinearVelocity(vec2);

	//this->physicsBody->ApplyLinearImpulseToCenter(vec2, true);

	//std:cout << "angle is " << this->physicsBody->GetAngle() << "\n";


}

void PlayerObject::addWeapon(string bulletGameObjectId, float xWeaponOffsetPct, float yWeaponOffsetPct)
{

	this->weapon = new Weapon();
	this->weapon->init(bulletGameObjectId, this, xWeaponOffsetPct, yWeaponOffsetPct);

}



