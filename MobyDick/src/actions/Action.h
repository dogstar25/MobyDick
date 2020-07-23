#pragma once

#include <map>
#include <string>
#include <memory>

#include "../components/TransformComponent.h"
#include "../components/AnimationComponent.h"
#include "../components/PhysicsComponent.h"
#include "../ecs/Types.hpp"


class Action
{
public:

	Action();
	~Action();
	virtual void perform(){};
	virtual void perform(Entity) {};
	virtual void perform(Entity, int, int) {};
	virtual void perform(Entity, float) {};

};

