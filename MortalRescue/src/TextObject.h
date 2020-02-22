#pragma once
#include "GameObject.h"

class TextObject :	public GameObject
{
public:
	TextObject(string, float, float, float);
	virtual ~TextObject();

	void update();
	void render();
	SDL_Rect getPositionRect();
	SDL_Rect getRenderDestRect();
	Texture* updateDynamicTextTexture();
	Texture* generateTextTexture();



	bool isDynamic;
	string fontId;
	string textValue;
		

};

