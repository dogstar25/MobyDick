#pragma once
#include "GameObject.h"

class TextObject :	public GameObject
{
public:
	TextObject(string, float, float, float);
	virtual ~TextObject();

	void update();
	void render();
	SDL_Rect getRenderDestRect();

	bool isDynamic;
	string fontId;
	string textValue;
		

};

