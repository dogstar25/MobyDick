#include "TextObject.h"
#include "game.h"

TextObject::TextObject() 
{
}

TextObject::TextObject(string gameObjectId, int xMapPos, int yMapPos, int angleAdjust) :
	GameObject(gameObjectId, xMapPos, yMapPos, angleAdjust)
{

}


TextObject::~TextObject()
{
}

void TextObject::update()
{

	//if dynamic go get new text

}

void TextObject::render()
{

	game->textureManager.render(this);

}



