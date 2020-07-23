#pragma once
#include "../ecs/System.hpp"

class PlayerControlSystem :  public System
{
public:

	void update();
	void init();
};

