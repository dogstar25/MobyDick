#pragma once

#include <map>
#include <string>
#include <memory>

#include "../components/TransformComponent.h"
#include "../components/AnimationComponent.h"
#include "../components/PhysicsComponent.h"


class GameObject;

class Action
{
public:

	Action();
	~Action();
	virtual void perform() {};
	virtual void perform(
		std::shared_ptr<PhysicsComponent> physicsComponent, 
		std::shared_ptr<AnimationComponent> animationComponent) {};
	virtual void perform(
		std::shared_ptr<PhysicsComponent> physicsComponent,
		std::shared_ptr<AnimationComponent> animationComponent,
		float, int, int) {};
	virtual void perform(
		std::shared_ptr<PhysicsComponent> physicsComponent,
		std::shared_ptr<AnimationComponent> animationComponent,
		float) {};

};

