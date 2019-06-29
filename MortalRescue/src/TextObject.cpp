#include "TextObject.h"
#include "game.h"

TextObject::TextObject() 
{
}

TextObject::TextObject(string gameObjectId, int xMapPos, int yMapPos, int angleAdjust) :
	GameObject(gameObjectId, xMapPos, yMapPos, angleAdjust)
{

	this->isDynamic = this->definition->isDynamicText;

}


TextObject::~TextObject()
{
}

void TextObject::update()
{

	//if dynamic go get new text
	if (this->isDynamic == true)
	{
		this->texture = game->textureManager.updateDynamicTextTexture(this);
		 
	}

}

void TextObject::render()
{

	game->textureManager.render(this);

}



