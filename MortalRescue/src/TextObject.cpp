#include "TextObject.h"
#include "game.h"


TextObject::TextObject(string gameObjectId, int xMapPos, int yMapPos, int angleAdjust) :
	GameObject(gameObjectId, xMapPos, yMapPos, angleAdjust)
{

	this->isDynamic = this->definition->textDetails.isDynamic;
	this->textValue = this->definition->textDetails.value;

	//Use the text size value for both x and y size of the gameobject
	this->xSize = this->definition->textDetails.size;
	this->ySize = this->definition->textDetails.size;

	this->color = { this->definition->textDetails.color.r,
		this->definition->textDetails.color.g,
		this->definition->textDetails.color.b,
		this->definition->textDetails.color.a };

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

SDL_Rect TextObject::getRenderDestRect()
{
	SDL_Rect destRect;

	//calculate the destination rectangle - must convert meters to pixels with scale factor
	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(this->texture->sdlTexture, NULL, NULL, &texW, &texH);

	destRect.w = texW;
	destRect.h = texH;
	destRect.x = this->xPos;
	destRect.y = this->yPos;

	return destRect;

}

void TextObject::render()
{

	SDL_Rect *textureSourceRect = NULL, destRect;
	SDL_Texture* texture = NULL;

	//Get render destination rectangle
	destRect = this->getRenderDestRect();

	//Get texture
	texture = GameObject::getRenderTexture(texture);

	//Get render texture src rectangle
	textureSourceRect = this->getRenderTextureRect(textureSourceRect);

	game->textureManager.render(texture, textureSourceRect, &destRect, 0);


}



