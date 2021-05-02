#pragma once
#include <string>

class GameObject;

class Action
{
public:

	Action() {}
	~Action();
	virtual void perform() {};
	virtual void perform(GameObject*) {};

};

