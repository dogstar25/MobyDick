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

	printf("SDL Delta Ticks %d\n", Game::clock.delta);
	this->xPos += this->xVelocity * (this->speed * Game::clock.delta);
	this->yPos += this->yVelocity * (this->speed * Game::clock.delta);


}

void GameObject::init()
{

	this->xVelocity = 0;;
	this->yVelocity = 0;;

}




