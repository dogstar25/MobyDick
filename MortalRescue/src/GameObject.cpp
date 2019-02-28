#include "GameObject.h"
#include "game.h"


void GameObject::update()
{
	//If this object is animated, then animate it
	if(this->definition->isAnimated) {
		this->definition->animations[this->currentAnimationState]->animate(this);
	}


}

GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}





