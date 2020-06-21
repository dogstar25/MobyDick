#pragma once
#include "GameObject.h"

class TextObject :	public GameObject
{
public:
	TextObject(std::string, float, float, float);
	virtual ~TextObject();

	void update();
	void render();
	SDL_Rect getPositionRect();
	SDL_Rect getRenderDestRect();
	std::shared_ptr<Texture> updateDynamicTextTexture();
	std::shared_ptr<Texture> generateTextTexture();

	bool isDynamic;
	std::string fontId;
	std::string textValue;
		

};

