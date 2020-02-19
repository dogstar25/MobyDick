#include "TextObject.h"
#include "texture.h"
#include "game.h"


TextObject::TextObject(string gameObjectId, float xMapPos, float yMapPos, float angleAdjust) :
	GameObject(gameObjectId, xMapPos, yMapPos, angleAdjust)
{

	this->isDynamic = this->definition->textDetails.isDynamic;
	this->textValue = this->definition->textDetails.value;

	//Use the text size value for both x and y size of the gameobject
	this->xSize = this->definition->textDetails.size;
	this->ySize = this->definition->textDetails.size;

	//If the text item is actually smaller than the gridsize, adjust the position to reflect that
	float adjPosX=0;
	float adjPosY=0;
	/*
	if (this->xSize < game->worldGridSize.w)
	{
		this->xPos -= (game->worldGridSize.w) - this->xSize);
	}
	
	if (this->ySize < game->worldGridSize.h)
	{
		this->yPos -= (game->worldGridSize.h - (this->ySize+ game->worldGridSize.h));
	}
	*/
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

SDL_Rect TextObject::getPositionRect()
{
	SDL_Rect positionRect;

	//calculate the destination rectangle
	//Use query texture to get size of the generated text object
	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(this->texture->sdlTexture, NULL, NULL, &texW, &texH);

	positionRect.w = texW;
	positionRect.h = texH;
	positionRect.x = this->xPos;
	positionRect.y = this->yPos;

	return positionRect;
}

SDL_Rect TextObject::getRenderDestRect()
{
	SDL_Rect destRect;

	//Get the position/size rectangle of the object
	destRect = this->getPositionRect();

	//adjust render position X and Y for camera if not an absolute positioned object
	if (this->definition->absolutePositioning == false)
	{
		destRect.x -= game->camera.frame.x;
		destRect.y -= game->camera.frame.y;
	}

	return destRect;

}

void TextObject::render()
{

	SDL_Rect* textureSourceRect=NULL, destRect;
	SDL_Texture* texture = NULL;

	//Get render destination rectangle
	destRect = this->getRenderDestRect();

	//Get texture
	texture = GameObject::getRenderTexture();

	//Get render texture src rectangle
	textureSourceRect = this->getRenderTextureRect();

	game->textureManager.render(texture, this->color, textureSourceRect, &destRect, 0);


}



