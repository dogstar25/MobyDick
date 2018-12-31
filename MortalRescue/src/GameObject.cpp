#include "GameObject.h"
#include "game.h"
#include <SDL.h>




GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

void GameObject::handleEvent(SDL_Event* event)
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

void GameObject::update()
{

	float acceleration = this->playerSpeed;
	float velocity = (float)(acceleration * (Game::clock.time_diff.count())*1000);
	//float velocity = this->playerSpeed;
	
	
	//this->xPos += velocity * this->xDirection;
	//this->yPos += velocity * this->yDirection;

	//Set the same for physics body
	if (this->isPhysicsObject)
	{
		float32 x, y;
		x = velocity * this->xDirection;
		y = velocity * this->yDirection;
		b2Vec2 vec2 = b2Vec2(x, y);
		//this->physicsBody->SetTransform(vec2, this->physicsBody->GetAngle());
		this->physicsBody->ApplyLinearImpulseToCenter(vec2, true);
		
	}


	//std::cout << "xPos is " << this->physicsBody->GetPosition().x << " \n";


}

void GameObject::init()
{

	this->xDirection = 1;
	this->yDirection = 1;

}




