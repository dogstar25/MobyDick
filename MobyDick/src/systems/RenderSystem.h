#pragma once
#include "../ecs/System.hpp"

class RenderSystem : public System
{
public:
	void init();

	void update();
	void render();

private:

};

