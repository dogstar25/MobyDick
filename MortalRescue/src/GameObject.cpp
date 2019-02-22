#include "GameObject.h"
#include "game.h"


void GameObject::update()
{
	//If this object is animated, then animate it
	if(this->definition->isAnimated) {
		this->definition->animations[this->currentAnimationState]->animate(this);
	}

	//float test = RADIANS_TO_DEGREES(2);
	//Calculate the physics angle for the rendering later
	//float angle = this->physicsBody->GetAngle();
	//angle = angle * 180 / M_PI;
	//this->angle = angle;

}





