#include "HeroMoveAction.h"
#include "HeroFireAction.h"
#include <iostream>
#include <memory>

#include "../game.h"
#include "../components/PhysicsComponent.h"
#include "../components/AnimationComponent.h"
#include "../components/VitalityComponent.h"


HeroMoveAction::HeroMoveAction()
{

}

HeroMoveAction::~HeroMoveAction()
{


}

void HeroMoveAction::perform(Entity entity, int direction, int strafe)
{
	auto& physicsComponent = Game::instance().gameCoordinator().GetComponent<PhysicsComponent>(entity);
	auto& vitalityComponent = Game::instance().gameCoordinator().GetComponent<VitalityComponent>(entity);

	//Move
	physicsComponent.applyMovement(vitalityComponent.speed, direction, strafe);

	//Animate
	if (Game::instance().gameCoordinator().hasComponent<AnimationComponent>(entity))
	{
		auto& animationComponent = Game::instance().gameCoordinator().GetComponent<AnimationComponent>(entity);

		if (direction == 0 && strafe == 0)
		{
			animationComponent.m_currentAnimationState = ANIMATION_IDLE;
		}
		else
		{
			animationComponent.m_currentAnimationState = ANIMATION_RUN;
		}

	}

}