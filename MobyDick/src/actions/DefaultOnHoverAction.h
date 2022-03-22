#pragma once
#include "Action.h"

class DefaultOnHoverAction : public Action
{

public:
	DefaultOnHoverAction() = default;

	void perform(GameObject* gameObject) override;

private:



};

