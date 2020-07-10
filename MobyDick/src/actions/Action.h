#pragma once

#include <map>
#include <string>
#include <memory>


class GameObject;

class Action
{
public:

	Action();
	~Action();
	virtual void perform(GameObject* gameObject) {};
	virtual void perform(GameObject* gameObject, int direction, int strafe) {};

};

