#pragma once
#include "../ecs/System.hpp"

class PlayerControlSystem :  public System
{
public:
	PlayerControlSystem();
	~PlayerControlSystem();

	void update();
	void init();
};

