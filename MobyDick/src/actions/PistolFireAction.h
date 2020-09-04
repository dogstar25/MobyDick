#pragma once
#include "Action.h"
class PistolFireAction : public Action
{
public:
	PistolFireAction();
	~PistolFireAction();

	void perform(GameObject* gameObject) override;

private:

};

