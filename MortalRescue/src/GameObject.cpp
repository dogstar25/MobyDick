#include "GameObject.h"


void GameObject::update()
{
	//If this object is animated, then animate it
	if(this->definition->isAnimated) {
		this->animations[this->currentAnimationState].animate(this);
	}

	//Calculate the physics angle for the rendering later
	//float angle = this->physicsBody->GetAngle();
	//angle = angle * 180 / M_PI;
	//this->angle = angle;

}





