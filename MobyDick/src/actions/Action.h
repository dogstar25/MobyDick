#pragma once
#include <string>

class GameObject;

class Action
{
public:

	Action() {}
	~Action();
	virtual void perform(GameObject*) {};

	virtual void setMoveParms(int direction, int strafe) {};
	virtual void setRotateParms(float angularVelocity) {};
	virtual void setUseParms(std::string) {};

};

