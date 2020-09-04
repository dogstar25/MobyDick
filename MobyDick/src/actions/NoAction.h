#pragma once
#include "Action.h"
#include <string>

class NoAction :   public Action
{
public:
	NoAction() {};
	~NoAction() {};

	void perform(GameObject* gameObject) override {};

};

