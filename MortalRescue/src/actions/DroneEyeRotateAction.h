#pragma once
#include "actions/Action.h"

class DroneEyeRotateAction : public Action
{

public:
	DroneEyeRotateAction() = default;

	void perform(GameObject* gameObject, float angularVelocity) override;


};