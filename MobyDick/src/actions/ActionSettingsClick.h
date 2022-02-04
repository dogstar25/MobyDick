#pragma once

#include "Action.h"
class ActionSettingsClick : public Action
{

public:

	ActionSettingsClick();
	~ActionSettingsClick();

	void perform(GameObject* gameObject) override;

private:

};


