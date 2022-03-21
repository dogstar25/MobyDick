#pragma once
#include "Action.h"
class DefaultRotateAction : public Action
{

public:
	DefaultRotateAction() = default;

	void perform(GameObject* gameObject, Json::Value actionParms) override;

};

