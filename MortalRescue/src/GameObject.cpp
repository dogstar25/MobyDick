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


}
GameObject::GameObject()
{

}

GameObject::GameObject(string gameObjectId, int xMapPos, int yMapPos, int angleAdjust)
{
	this->definition = game->gameObjectManager.gameObjectDefinitions[gameObjectId];;

	//Apply gameobject definition overrides values if passed
	// usually for particular game tile position and angle
	if (angleAdjust != 0)
	{
		this->angleAdjustment = angleAdjust;
	}

	//Gameobject must be passed in it's starting position
	//Multiply the size times the x,y position in the map grid that represents the world
	//When buildB2Body executes, it will build the box2d object centered on the x,y position we give,
	// We need it centered on the grid location
	//so add half of the object size so that the object will be placed with its top left corner in the grid location
	//we specify
	this->definition->initPosX = (xMapPos * this->definition->xSize) + (this->definition->xSize / 2);
	this->definition->initPosY = (yMapPos * this->definition->ySize) + (this->definition->ySize / 2);

	//Get pointer to the texture
	this->staticTexture = game->textureManager.getTexture(this->definition->texture)->sdlTexture;

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







