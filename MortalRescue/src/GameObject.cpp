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
			this->yVelocity = -1;
			break;
		case SDLK_s:
			this->yVelocity = 1;
			break;
		case SDLK_a:
			this->xVelocity = -1;
			break;
		case SDLK_d:
			this->xVelocity = 1;
			break;
		}
	}
	else if (event->type == SDL_KEYUP) {

		switch (event->key.keysym.sym) {
		case SDLK_w:
			this->yVelocity = 0;
			break;
		case SDLK_s:
			this->yVelocity = 0;
		case SDLK_a:
			this->xVelocity = 0;
		case SDLK_d:
			this->xVelocity = 0;
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
	this->xPos += round(this->xVelocity * (this->speed));
	this->yPos += round(this->yVelocity * (this->speed));

	//printf("Performance COunter is  %d\n", Game::clock.NOW);
	//printf("Performance Frequency is %d\n", Game::clock.PerfFreq);


}

void GameObject::init()
{

	this->xVelocity = 0;;
	this->yVelocity = 0;;

}




