#pragma once
#include "Action.h"

class DefaultOnHoverAction : public Action
{

public:
	DefaultOnHoverAction();
	~DefaultOnHoverAction();

	void perform(GameObject* gameObject) override;

private:



};

