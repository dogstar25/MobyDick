#include "GameObject.h"
#include "game.h"
#include "Weapon.h"


void GameObject::update()
{
	//If this object is animated, then animate it
	if(this->definition->isAnimated) {
		this->definition->animations[this->currentAnimationState]->animate(this);
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

	//is this a debug object then get the default debug definition but change its 
	//id value to the one we passed in
	if (gameObjectId.rfind("DEBUG_", 0) == 0)
	{
		this->definition = game->gameObjectManager.gameObjectDefinitions["DEBUG_ITEM"];
		this->definition->id = gameObjectId;
	}
	else
	{
		this->definition = game->gameObjectManager.gameObjectDefinitions[gameObjectId];
	}

	this->angle = angleAdjust;

	this->xPos = xMapPos * game->worldGridSize.w;
	this->yPos = yMapPos * game->worldGridSize.h;

	//Get pointer to the texture
	this->texture = game->textureManager.getTexture(this->definition->textureId);

}

GameObject::~GameObject()
{
}

GameObjectDefinition::GameObjectDefinition()
{
}

GameObjectDefinition::~GameObjectDefinition()
{
	
	for (auto animation : this->animations)
	{
		delete animation.second;
	}

	this->animations.clear();
	
}







