#pragma once
#include "Action.h"
class QuitGameAction : public Action
{

public:

	QuitGameAction();
	~QuitGameAction();

	void perform() override;

private:

};

