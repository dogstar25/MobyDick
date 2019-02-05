#include "PlayerObject.h"
#include <math.h>

void PlayerObject::handlePlayerMovementEvent(SDL_Event* event)
{

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
			this->direction = 1;
			break;
		case SDLK_s:
			//this->yDirection = 1;
			this->direction = -1;
			break;
		case SDLK_a:
			//this->xDirection = -1;
			this->strafe = -1;
			break;
		case SDLK_d:
			//this->xDirection = 1;
			this->strafe = +1;
			break;
		}
	}

	if (event->type == SDL_MOUSEMOTION) {

		//method 1
		float angularVelocity = event->motion.xrel * Game::config.mouseSensitivity;

		this->physicsBody->SetAngularVelocity(angularVelocity);
		/*
		if (this->physicsBody->GetAngle() * 180 / M_PI > 360) {
			this->physicsBody->SetTransform(this->physicsBody->GetPosition(), 
				(this->physicsBody->GetAngle() * 180 / M_PI) -360);
		}
		if (this->physicsBody->GetAngle() * 180 / M_PI < -360) {
			this->physicsBody->SetTransform(this->physicsBody->GetPosition(),
				(this->physicsBody->GetAngle() * 180 / M_PI) + 360);
		}
		*/
		//method 2
		//float desiredAngle = atan2f(-event->motion.x, -event->motion.y);
		//this->physicsBody->SetTransform(this->physicsBody->GetPosition(), desiredAngle);

		std::cout << "xmouse is " << event->motion.x << " ymouse is " << event->motion.y << "\n";
		//std::cout << "player angle is " << this->physicsBody->GetAngle() * 180 / M_PI << "\n";

	}
}

void PlayerObject::updatePlayer()
{
	//Call base game object update
	this->update();

	//Call all player movement updates
	this->updatePlayerMovement();

}

void PlayerObject::updatePlayerMovement()
{
	/*
	Lots of tweeks can be down here depending on the game you are building for smoother movement
	*/

	float acceleration = this->definition.playerSpeed;
	//float acceleration = this->definition.playerSpeed + (game->awakeCount * .05);
	float velocity = min((float)0.1, (float)(acceleration * (Game::clock.time_diff.count() * 100000)));
	//float velocity = acceleration * (Game::clock.time_diff.count() * 100000);
	//cout << "velocity is " << velocity << "\n";
	//float velocity = this->definition.playerSpeed +(game->awakeCount * .02);


	//float32 x = velocity * this->xDirection;
	//float32 y = velocity * this->yDirection;

	//Calc direction XY
	float dx = cos(this->physicsBody->GetAngle()) * velocity * this->direction; // X-component.
	float dy = sin(this->physicsBody->GetAngle()) * velocity * this->direction; // Y-component.

	//calc strafe xy and add direction and strafe vectors
	//1.578 is 90 degrees
	float sx = cos(this->physicsBody->GetAngle() + 1.5708) * velocity * this->strafe; // X-component.
	float sy = sin(this->physicsBody->GetAngle() + 1.5708) * velocity * this->strafe; // Y-component.



	//posX += Math.cos(rotation) *  forwardSpeed + Math.sin(rotation) * strafeSpeed;
	//posY -= -Math.cos(rotation) * strafeSpeed + Math.sin(rotation) * forwardSpeed;
	//x = (cos(this->physicsBody->GetAngle()) *  velocity + sin(this->physicsBody->GetAngle())) * this->xDirection;
	//y = sin(this->physicsBody->GetAngle()) * this->yDirection + sin(this->physicsBody->GetAngle()) * velocity * -1;

	//Create the vector for forward/backward  direction
	b2Vec2 directionVector = b2Vec2(dx, dy);

	//Create the vector for strafe direction
	b2Vec2 strafeVector = b2Vec2(sx, sy);

	//Initialize new final movemtn vector
	b2Vec2 vec2;
	vec2.SetZero();

	vec2 = (directionVector + strafeVector);

	//this->physicsBody->SetTransform(vec3, this->physicsBody->GetAngle());
	this->physicsBody->SetLinearVelocity(vec2);

	//this->physicsBody->ApplyLinearImpulseToCenter(vec3, true);

	//std:cout << "strafevector length is " << strafeVector.Length() << "\n";

}
