#pragma once
#include "actions/RotateAction.h"

class DroneEyeRotateAction : public RotateAction
{

public:
	DroneEyeRotateAction();
	DroneEyeRotateAction(float angularVelocity);
	~DroneEyeRotateAction();

	void perform(GameObject* gameObject) override;


};