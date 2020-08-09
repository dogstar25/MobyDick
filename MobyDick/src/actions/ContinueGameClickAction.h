#pragma once
#include "Action.h"
class ContinueGameClickAction :  public Action
{
public:

	ContinueGameClickAction();
	~ContinueGameClickAction();

	void perform(GameObject* gameObject) override;

private:

};

