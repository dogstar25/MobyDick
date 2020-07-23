#include "HeroRotateAction.h"

#include <iostream>

#include "../game.h"
#include "../components/PhysicsComponent.h"
#include "../components/AnimationComponent.h"



HeroRotateAction::HeroRotateAction()
{

}

HeroRotateAction::~HeroRotateAction()
{


}

void HeroRotateAction::perform(Entity entity, float angularVelocity )
{
	auto& physicsComponent = Game::instance().gameCoordinator().GetComponent<PhysicsComponent>(entity);

	//Rotate
	physicsComponent.applyRotation(angularVelocity);

	//Animate
	if (Game::instance().gameCoordinator().hasComponent<AnimationComponent>(entity))
	{
		auto& animationComponent = Game::instance().gameCoordinator().GetComponent<AnimationComponent>(entity);
		animationComponent.m_currentAnimationState = ANIMATION_RUN;
	}
}
