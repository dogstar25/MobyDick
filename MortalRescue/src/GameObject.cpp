#include "GameObject.h"
#include "game.h"
#include <SDL.h>


void GameObject::handlePlayerMovementEvent(SDL_Event* event)
{

	if (event->type == SDL_KEYDOWN) {
		switch (event->key.keysym.sym) {
		case SDLK_w:
			this->yDirection = -1;
			break;
		case SDLK_s:
			this->yDirection = 1;
			break;
		case SDLK_a:
			this->xDirection = -1;
			break;
		case SDLK_d:
			this->xDirection = 1;
			break;
		}
	}
	else if (event->type == SDL_KEYUP) {

		switch (event->key.keysym.sym) {
		case SDLK_w:
			this->yDirection = 0;
			break;
		case SDLK_s:
			this->yDirection = 0;
		case SDLK_a:
			this->xDirection = 0;
		case SDLK_d:
			this->xDirection = 0;
			break;
		}

	}
}

void GameObject::updatePlayer()
{

	float acceleration = this->definition.playerSpeed;
	//float velocity = (float)(acceleration * (Game::clock.time_diff.count())*100);
	float velocity = this->definition.playerSpeed;
	
	
	//this->xPos += velocity * this->xDirection;
	//this->yPos += velocity * this->yDirection;

	//Set the same for physics body
	if (this->definition.isPhysicsObject)
	{
		float32 x, y;
		x = velocity * this->xDirection;
		y = velocity * this->yDirection;
		//b2Vec2 vec2 = b2Vec2(x, y);

		//x = this->physicsBody->GetPosition().x + velocity * this->xDirection;
		//y = this->physicsBody->GetPosition().y + velocity * this->yDirection;
		b2Vec2 vec3 = b2Vec2(x, y);

		//this->physicsBody->SetTransform(vec3, this->physicsBody->GetAngle());
		this->physicsBody->SetLinearVelocity(vec3);
		//this->physicsBody->ApplyLinearImpulseToCenter(vec2, true);
		//this->physicsBody->SetLinearVelocity(vec2);
		
	}


	//std::cout << "xPos is " << this->physicsBody->GetPosition().x << " \n";


}

void GameObject::update()
{
	if(this->definition.isAnimated) {
		this->animations[this->currentAnimationState].animate(this);
	}
}





