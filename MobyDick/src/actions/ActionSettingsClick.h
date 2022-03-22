#pragma once

#include "Action.h"
class ActionSettingsClick : public Action
{

public:

	ActionSettingsClick() = default;

	void perform(GameObject* gameObject) override;

private:

};


