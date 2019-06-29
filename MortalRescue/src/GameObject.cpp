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
	this->definition = game->gameObjectManager.gameObjectDefinitions[gameObjectId];;

	this->angle = angleAdjust;

	this->xPos = xMapPos * game->worldGridSize.w;
	this->yPos = yMapPos * game->worldGridSize.h;

	//Get pointer to the texture
	this->texture = game->textureManager.getTexture(this->definition->texture);

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







