#include "TextObject.h"
#include "Texture.h"
#include "TextureManager.h"
#include "DynamicTextManager.h"
#include "Game.h"

extern Game* game;


TextObject::TextObject(string gameObjectId, float xMapPos, float yMapPos, float angleAdjust) :
	GameObject(gameObjectId, xMapPos, yMapPos, angleAdjust)
{

	this->isDynamic = this->definition()->textDetails.isDynamic;
	this->textValue = this->definition()->textDetails.value;

	//Use the text size value for both x and y size of the gameobject
	this->setSize(this->definition()->textDetails.size, 
		this->definition()->textDetails.size);

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
	this->setColor(this->definition()->textDetails.color.r,
		this->definition()->textDetails.color.g,
		this->definition()->textDetails.color.b,
		this->definition()->textDetails.color.a);

	//Get or Generate the text texture
	this->setTexture( generateTextTexture() );


}


TextObject::~TextObject()
{
}

void TextObject::update()
{

	if (this->isDynamic == true)
	{

		this->setTexture( updateDynamicTextTexture());
	}

}

SDL_Rect TextObject::getPositionRect()
{
	SDL_Rect positionRect;

	//calculate the destination rectangle
	//Use query texture to get size of the generated text object
	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(this->texture()->sdlTexture, NULL, NULL, &texW, &texH);

	positionRect.w = texW;
	positionRect.h = texH;
	positionRect.x = position().x;
	positionRect.y = position().y;

	return positionRect;
}

SDL_Rect TextObject::getRenderDestRect()
{
	SDL_Rect destRect;

	//Get the position/size rectangle of the object
	destRect = this->getPositionRect();

	//adjust render position X and Y for camera if not an absolute positioned object
	if (this->definition()->absolutePositioning == false)
	{
		destRect.x -= Camera::instance().frame().x;
		destRect.y -= Camera::instance().frame().y;
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

	TextureManager::instance().render(texture, this->color(), textureSourceRect, &destRect, 0);


}

Texture* TextObject::generateTextTexture()
{

	SDL_Surface* surface;
	Texture* texture = new Texture();
	SDL_Texture* sdlTexture;

	int textSize = this->definition()->textDetails.size; // default to x size
	string fontFile = TextureManager::instance().getFont(this->fontId);

	TTF_Font* fontObject = TTF_OpenFont(fontFile.c_str(), textSize);
	//surface = TTF_RenderText_Solid(fontObject, textObject->textValue.c_str(), color);
	surface = TTF_RenderText_Blended(fontObject, this->textValue.c_str(), this->color());
	TTF_CloseFont(fontObject);
	string test = TTF_GetError();

	//Set the size of the textObject now that its texture has been generated
	if (surface != NULL)
	{
		this->setSize(surface->w, surface->h);
	}

	sdlTexture = TextureManager::instance().createTextureFromSurface(surface);
	SDL_FreeSurface(surface);

	texture->sdlTexture = sdlTexture;

	//Add it to the main texture map
	//Append "TEXTURE" to gameobejct id to create textture id
	//possible memory leak - delete texture before adding in case it is already there
	//string textureId = textObject->id + "_TEXT_TEXTURE";
	//this->addTexture(textureId, texture);
	return 	texture;

}


Texture* TextObject::updateDynamicTextTexture()
{

	textItem* newText;
	Texture* textureObject;
	SDL_Surface* surface;

	//newText = game->dynamicTextManager.textItems[gameObject->definition->id].get();
	newText = DynamicTextManager::instance().getTextItem(this->definitionId());

	//check the clock and see if enough time as gone by
	steady_clock::time_point now_time = steady_clock::now();
	std::chrono::duration<double> time_diff = now_time - newText->time_snapshot;

	//FIXME: .2 needs to be a setting somewhere
	if (newText->hasChanged == true && time_diff.count() > .2)
	{
		//update the timestamp
		newText->time_snapshot = now_time;

		//Destroy this texture from the map before we generate a new one - memory leak otherwise
		SDL_DestroyTexture(this->texture()->sdlTexture);

		//Build new texture
		this->textValue = newText->text;
		textureObject = generateTextTexture();
		newText->hasChanged = false;

	}
	else
	{
		textureObject = this->texture();

		//TODO:set a flag at game object level so that the TextureManager::render(TextObject* gameObject) doesnt have to 
		// do SDL_QueryTexture
	}

	return textureObject;

}


