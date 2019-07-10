#include "TextObject.h"
#include "GameObjectDefinition.h"
#include "game.h"

TextObject::TextObject() 
{
}

TextObject::TextObject(string gameObjectId, int xMapPos, int yMapPos, int angleAdjust) :
	GameObject(gameObjectId, xMapPos, yMapPos, angleAdjust)
{

	this->isDynamic = definition->textDetails.isDynamic;
	this->textValue = definition->textDetails.label;

	//Use the text size value for both x and y size of the gameobject
	this->xSize = definition->textDetails.size;
	this->ySize = definition->textDetails.size;

	this->color = { definition->textDetails.color.r,
		definition->textDetails.color.g,
		definition->textDetails.color.b,
		definition->textDetails.color.a };

	//Get or Generate the text texture
	this->texture = game->textureManager.generateTextTexture(this);


}


TextObject::~TextObject()
{
}

void TextObject::update()
{

	if (this->isDynamic == true)
	{
		this->texture = game->textureManager.updateDynamicTextTexture(this);
	}

}

void TextObject::render()
{

	game->textureManager.render(this);

}



