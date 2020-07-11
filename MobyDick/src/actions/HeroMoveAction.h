#pragma once
#include "Action.h"
class HeroMoveAction : public Action
{

public:
	HeroMoveAction();
	~HeroMoveAction();

	void perform(
		std::shared_ptr<PhysicsComponent> physicsComponent,
		std::shared_ptr<AnimationComponent> animationComponent, 
		float speed, 
		int direction, 
		int strafe) override;


};

