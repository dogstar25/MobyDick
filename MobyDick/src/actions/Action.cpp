#include "Action.h"
#include <iostream>
#include "../GameObject.h"

#include "HeroFireAction.h"

Action::Action()
{

}

Action::~Action()
{

}

void Action::perform(GameObject* gameObject)
{
	std::cout << "ACTION";

}

