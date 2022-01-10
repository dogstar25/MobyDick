#pragma once
#include "RotateAction.h"
class DefaultRotateAction : public RotateAction
{

public:
	DefaultRotateAction();
	DefaultRotateAction(float angularVelocity);
	~DefaultRotateAction();

	void perform(GameObject* gameObject) override;

};

