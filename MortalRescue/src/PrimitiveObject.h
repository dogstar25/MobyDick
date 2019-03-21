#pragma once
#include "GameObject.h"

class PrimitiveObject : public GameObject
{
public:
	PrimitiveObject();
	~PrimitiveObject();

	void update();
};

