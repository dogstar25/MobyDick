#pragma once
#include "RotateAction.h"

class ActorRotateAction : public RotateAction
{

public:
	ActorRotateAction();
	ActorRotateAction(float angularVelocity);
	~ActorRotateAction();

	void perform(GameObject* gameObject) override;


};

