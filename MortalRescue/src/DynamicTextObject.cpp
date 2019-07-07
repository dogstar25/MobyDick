#include "DynamicTextObject.h"
#include "game.h"


DynamicTextObject::DynamicTextObject()
{
}

DynamicTextObject::DynamicTextObject(string gameObjectId, int xMapPos, int yMapPos, int angleAdjust) :
	GameObject(gameObjectId, xMapPos, yMapPos, angleAdjust)
{

	//Get or Generate the text texture
	this->texture = game->textureManager.getTexture(this);


}


DynamicTextObject::~DynamicTextObject()
{
}

void DynamicTextObject::update()
{

	this->texture = game->textureManager.updateDynamicTextTexture(this);

}

void DynamicTextObject::render()
{

	game->textureManager.render(this);

}
