#include "TextObject.h"
#include "game.h"

TextObject::TextObject() 
{
}

TextObject::TextObject(string gameObjectId, int xMapPos, int yMapPos, int angleAdjust) :
	GameObject(gameObjectId, xMapPos, yMapPos, angleAdjust)
{

	//Get or Generate the text texture
	this->texture = game->textureManager.getTexture(this);


}


TextObject::~TextObject()
{
}

void TextObject::update()
{

}

void TextObject::render()
{

	game->textureManager.render(this);

}



