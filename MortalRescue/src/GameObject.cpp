#include "GameObject.h"
#include "game.h"
#include "Weapon.h"
#include "GameObjectDefinition.h"
#include "GameObjectAnimation.h"


void GameObject::update()
{
	//If this object is animated, then animate it
	if(this->isAnimated) {
		this->animations[this->currentAnimationState]->animate(this);
	}


}

void GameObject::render()
{

	game->textureManager.render(this);

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







