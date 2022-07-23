#pragma once
#include "actions/Action.h"
class PistolSecondaryFireAction : public Action
{
public:
	PistolSecondaryFireAction() = default;

	void perform(GameObject* gameObject) override;

private:

};


