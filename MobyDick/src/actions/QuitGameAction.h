#pragma once
#include "Action.h"
class QuitGameAction : public Action
{

public:

	QuitGameAction() = default;

	void perform(GameObject* gameObject) override;

private:

};

