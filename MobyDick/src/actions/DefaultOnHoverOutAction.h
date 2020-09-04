#pragma once
#include "Action.h"
class DefaultOnHoverOutAction : public Action
{

public:
	DefaultOnHoverOutAction();
	~DefaultOnHoverOutAction();

	void perform(GameObject* gameObject) override;

private:


};

