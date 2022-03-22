#pragma once
#include "Action.h"
class ContinueGameClickAction :  public Action
{
public:

	ContinueGameClickAction() = default;

	void perform() override;

private:

};

