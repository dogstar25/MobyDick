#pragma once
#include "GameObject.h"

extern Game* game;

using namespace std;

class DynamicTextObject : public GameObject
{
public:
	DynamicTextObject();
	DynamicTextObject(string, int, int, int);
	~DynamicTextObject();

	void update();
	void render();

};

