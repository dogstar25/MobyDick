#pragma once
#include "GameObject.h"

class TextObject :	public GameObject
{
public:
	TextObject(string, int, int, int);
	virtual ~TextObject();

	void update();
	void render();
	SDL_Rect getRenderDestRect();

	bool isDynamic;
	string fontId;
	string textValue;
		

};

