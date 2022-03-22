#pragma once
#include "Action.h"
class DefaultOnHoverOutAction : public Action
{

public:
	DefaultOnHoverOutAction() = default;

	void perform(GameObject* gameObject) override;

private:


};

