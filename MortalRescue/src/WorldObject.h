#pragma once
#include "GameObject.h"


class WorldObject :	public GameObject
{
public:
	WorldObject();
	~WorldObject();

	void update();
	void render();

	b2Body* physicsBody;
};

