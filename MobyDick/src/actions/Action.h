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
	virtual void perform(){};
	virtual void perform(GameObject*) {};
	virtual void perform(GameObject*, int, int) {};
	virtual void perform(GameObject*, float) {};

};

