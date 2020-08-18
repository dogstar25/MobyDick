#pragma once
#include <string>

class GameObject;

class ContactEvent
{
public:

	ContactEvent() {}
	~ContactEvent();
	virtual void perform(GameObject* gameObject1, GameObject* gameObject2) {};

};

