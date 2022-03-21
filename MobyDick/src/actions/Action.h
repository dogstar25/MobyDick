#pragma once
#include <string>
#include <json/json.h>

class GameObject;

class Action
{
public:

	Action() {}
	~Action();
	virtual void perform() {};
	virtual void perform(GameObject*) {};
	virtual void perform(GameObject*, Json::Value actionParms) {};

};

