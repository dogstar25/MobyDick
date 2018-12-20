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

	//printf("SDL Delta Ticks %d\n", Game::clock.deltaTime);
	//this->xPos += this->xVelocity * (this->speed * Game::clock.deltaTime);
	//this->yPos += this->yVelocity * (this->speed * Game::clock.deltaTime);
	//printf("delta is %d\n", Game::clock.delta_time);
	float acceleration = this->speed;
	float velocity = (float)(acceleration * (Game::clock.time_diff.count() * 100));
	std::cout << "Acceleration is " << acceleration << " \n";
	//this->xPos += round(this->xVelocity * (this->speed * Game::clock.time_diff.count()));
	//this->yPos += round(this->yVelocity * (this->speed * Game::clock.time_diff.count()));
	this->xPos += velocity * this->xDirection;
	this->yPos += velocity * this->yDirection;
	std::cout << "xPos is " << this->xPos << " \n";


}

void GameObject::init()
{

	this->xDirection = 1;
	this->yDirection = 1;

}




