#include "TextObject.h"

#include "TextureManager.h"
#include "DynamicTextManager.h"
#include "GameConfig.h"
#include "Camera.h"



TextObject::TextObject(std::string gameObjectId, float xMapPos, float yMapPos, float angleAdjust) :
	GameObject(gameObjectId, xMapPos, yMapPos, angleAdjust)
{

	this->isDynamic = this->definition()->textDetails.isDynamic;

	//is this a debug object then set a flag and build the special debugId to be used in place 
	// of the gameObjectId for building textures
	if (gameObjectId.rfind("DEBUG_", 0) == 0)
	{
		this->isDebugText = true;
		this->debugId = gameObjectId;
	}

	//Text could be blank here if it's a dynamic text object
	if (this->definition()->textDetails.value.empty() == true)
	{
		this->textValue = "default";
	}
	else
	{
		this->textValue = this->definition()->textDetails.value;
	}

	this->setColor(this->definition()->textDetails.color.r,
		this->definition()->textDetails.color.g,
		this->definition()->textDetails.color.b,
		this->definition()->textDetails.color.a);

	//Get or Generate the text texture
	std::string textureId;
	if (this->isDebugText == true)
	{
		textureId = "TX_" + this->debugId;
	}
	else
	{
		textureId = "TX_" + this->definition()->id;
	}

	if (TextureManager::instance().hasTexture(textureId))
	{
		this->setTexture( TextureManager::instance().getTexture(textureId));
		this->setSize(this->getRenderSurface()->w, this->getRenderSurface()->h);

	}
	else
	{
		this->setTexture(generateTextTexture());
	}

	

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


std::shared_ptr<Texture> TextObject::generateTextTexture()
{

	std::shared_ptr<Texture> texture = std::make_shared<Texture>();;

	std::string textureId;
	if (this->isDebugText == true)
	{
		textureId = "TX_" + this->debugId;
	}
	else
	{
		textureId = "TX_" + this->definition()->id;
	}

	/*
	TextObjects have their textures generated here, not in the TextureManager.init().
	Generate the texture and add it to the TextureManager so that it can be shared with
	other objects if needed.
	*/
	SDL_Surface* tempSurface;

	int textSize = this->definition()->textDetails.size; // default to x size
	std::string fontFile = TextureManager::instance().getFont(this->fontId);

	TTF_Font* fontObject = TTF_OpenFont(fontFile.c_str(), textSize);
	tempSurface = TTF_RenderText_Blended(fontObject, this->textValue.c_str(), this->color());
	TTF_CloseFont(fontObject);

	//Set the size of the textObject now that its texture has been generated
	this->setSize(tempSurface->w, tempSurface->h);

	texture->sdlTexture = TextureManager::instance().createTextureFromSurface(tempSurface);
	texture->surface = tempSurface;
	//SDL_FreeSurface(tempSurface);

	//Add it to the textureManager but first free any texture that may already be there in case this is
	//being generated for a dynamic text object
	TextureManager::instance().addOrReplaceTexture(textureId, texture);

	return 	texture;

}


std::shared_ptr<Texture> TextObject::updateDynamicTextTexture()
{

	textItem* newText;
	std::shared_ptr<Texture> texture;
	SDL_Surface* surface;

	if (this->isDebugText == true)
	{
		newText = DynamicTextManager::instance().getTextItem(this->debugId);
	}
	else
	{
		newText = DynamicTextManager::instance().getTextItem(this->definition()->id);
	}

	//check the clock and see if enough time as gone by
	std::chrono::steady_clock::time_point now_time = std::chrono::steady_clock::now();
	std::chrono::duration<double> time_diff = now_time - newText->time_snapshot;

	//FIXME: .2 needs to be a setting somewhere
	//if (newText->hasChanged == true && time_diff.count() > 0)
	if (newText->hasChanged == true && time_diff.count() > GameConfig::instance().dynamicTextRefreshDelay())
	{
		//update the timestamp
		newText->time_snapshot = now_time;

		//Build new texture
		this->textValue = newText->text;
		texture = generateTextTexture();
		newText->hasChanged = false;

	}
	else
	{
		std::string textureId;
		if (this->isDebugText == true)
		{
			textureId = "TX_" + this->debugId;
		}
		else
		{
			textureId = "TX_" + this->definition()->id;
		}

		texture = TextureManager::instance().getTexture(textureId);

		//TODO:set a flag at game object level so that the TextureManager::render(TextObject* gameObject) doesnt have to 
		// do SDL_QueryTexture
	}

	return texture;

}


