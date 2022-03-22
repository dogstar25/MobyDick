#pragma once
#include "Action.h"
class LoadNextLevelAction :  public Action
{

public:

	LoadNextLevelAction() = default;

	void perform() override;

};

