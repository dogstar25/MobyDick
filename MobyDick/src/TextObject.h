#ifndef TEXT_OBJECT_H
#define TEXT_OBJECT_H

#include <memory>

#include "texture.h"
#include "GameObject.h"

class TextObject :	public GameObject
{
public:
	TextObject(std::string, float, float, float);
	virtual ~TextObject();

	void update();
	SDL_FRect getPositionRect();
	SDL_FRect getRenderDestRect();
	std::shared_ptr<Texture> updateDynamicTextTexture();
	std::shared_ptr<Texture> generateTextTexture();

	bool 
		isDynamic,
		isDebugText;
	std::string 
		fontId,
		textValue,
		debugId;
	
		

};

#endif