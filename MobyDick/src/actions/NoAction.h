#pragma once
#include "Action.h"
#include <string>

class NoAction :   public Action
{
public:
	NoAction() = default;

	void perform(GameObject* gameObject) override {};

};

