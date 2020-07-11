#include "HeroRotateAction.h"

#include <iostream>


HeroRotateAction::HeroRotateAction()
{

}

HeroRotateAction::~HeroRotateAction()
{


}

void HeroRotateAction::perform(std::shared_ptr<PhysicsComponent> physicsComponent,
	std::shared_ptr<AnimationComponent> animationComponent,
	float angularVelocity
	)
{

	physicsComponent->applyRotation(angularVelocity);

	if (animationComponent)
	{
		animationComponent->setCurrentAnimationState(ANIMATION_RUN);
	}
}
