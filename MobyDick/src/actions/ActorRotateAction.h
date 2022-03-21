#pragma once
#include "Action.h"
#include <json/json.h>

class ActorRotateAction : public Action
{

public:
	ActorRotateAction() = default;

	void perform(GameObject* gameObject, Json::Value actionParms) override;


};

