#include "GameObject.h"
#include "game.h"
#include "Weapon.h"
#include "GameObjectDefinition.h"
#include "Animation.h"


void GameObject::update()
{
	//If this object is animated, then animate it
	if(this->isAnimated) {
		this->animations[this->currentAnimationState]->animate(this);
	}


}

SDL_Rect* GameObject::getRenderDestRect(SDL_Rect *destRect)
{
	destRect->w = this->xSize * game->config.scaleFactor;
	destRect->h = this->ySize * game->config.scaleFactor;

	destRect->x = this->xPos;
	destRect->y = this->yPos;

	//Adjust position based on current camera position only if we want to
	if (this->definition->absolutePositioning == false)
	{
		destRect->x -= game->camera.frame.x;
		destRect->y -= game->camera.frame.y;
	}

	return destRect;

}

SDL_Rect*  GameObject::getRenderTextureRect(SDL_Rect* textureSrcRect)
{

	if (this->isAnimated) {

		textureSrcRect = &this->animations[this->currentAnimationState]->currentTextureAnimationSrcRect;
	}

	return textureSrcRect;

}

SDL_Texture * GameObject::getRenderTexture(SDL_Texture * aTexture)
{
	if (this->isAnimated) {

		aTexture = this->animations[this->currentAnimationState]->texture;
	}
	else {

		aTexture = this->texture->sdlTexture;
	}

	return aTexture;

}

void GameObject::render()
{
	SDL_Rect *textureSourceRect = NULL, destRect;
	SDL_Texture* texture=NULL;

	//Get render destination rectangle
	this->getRenderDestRect(&destRect);

	//Get texture
	texture = this->getRenderTexture(texture);

	//Get render texture src rectangle
	textureSourceRect = this->getRenderTextureRect(textureSourceRect);


	game->textureManager.renderTexture(texture, textureSourceRect, &destRect, 0);

}

GameObject::GameObject()
{

}

GameObject::GameObject(string gameObjectId, int xMapPos, int yMapPos, int angleAdjust)
{
	//set the id and it will be unique for every game object in the game
	this->id = gameObjectId + to_string(game->gameObjectCount);
	//this->id = gameObjectId;
	this->definitionId = gameObjectId;

	//is this a debug object then get the default debug definition but change its 
	//id value to the one we passed in
	if (gameObjectId.rfind("DEBUG_", 0) == 0)
	{
		//this->definition = game->gameObjectManager.gameObjectDefinitions["DEBUG_ITEM"];
		definition = game->gameObjectManager.gameObjectDefinitions["DEBUG_ITEM"];;
	}
	else
	{
		definition = game->gameObjectManager.gameObjectDefinitions[gameObjectId];
	}

	this->angle = angleAdjust;
	this->removeFromWorld = false;

	this->xPos = xMapPos * game->worldGridSize.w;
	this->yPos = yMapPos * game->worldGridSize.h;
	this->xSize = definition->xSize;
	this->ySize = definition->ySize;

	//Get pointer to the texture
	this->texture = game->textureManager.getTexture(definition->textureId);

	//get the animation objects
	if (definition->animations.size() > 0)
	{
		this->isAnimated = true;
		for (auto& animation : definition->animations) {

			//this->animations[animation.second.id] = animation.second;
			this->animations.emplace(animation.second->id, animation.second);

		}
	}



}

GameObject::~GameObject()
{

	this->animations.clear();


}







