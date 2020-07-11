#pragma once
#include "Action.h"
class HeroRotateAction : public Action
{

public:
	HeroRotateAction();
	~HeroRotateAction();

	void perform(
		std::shared_ptr<PhysicsComponent> physicsComponent,
		std::shared_ptr<AnimationComponent> animationComponent,
		float angularVelocity) override;

};

