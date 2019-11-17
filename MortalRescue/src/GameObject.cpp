#include "GameObject.h"

#include "game.h"
#include "Weapon.h"
#include "GameObjectDefinition.h"
#include "Animation.h"

#include "WorldObject.h"


void GameObject::update()
{
	//If this object is animated, then animate it
	if(this->definition->isAnimated) {
		this->animations[this->currentAnimationState]->animate(this);
	}

	//Loop through any possible child objects, in all 9 positions, and update their
	// position to reflect parent objects position
	if (this->definition->hasChildObjects == true)
	{
		updateChildObjects();
	}



}

SDL_Rect GameObject::getRenderDestRect()
{
	SDL_Rect destRect;

	destRect.w = this->xSize;
	destRect.h = this->ySize;

	destRect.x = this->xPos;
	destRect.y = this->yPos;

	//Adjust position based on current camera position only if we want to
	if (this->definition->absolutePositioning == false && this->isChildObject == false)
	{
		destRect.x -= game->camera.frame.x;
		destRect.y -= game->camera.frame.y;
	}

	return destRect;

}

SDL_Rect*  GameObject::getRenderTextureRect(SDL_Rect* textureSrcRect)
{

	if (this->definition->isAnimated) {

		textureSrcRect = &this->animations[this->currentAnimationState]->currentTextureAnimationSrcRect;
	}

	return textureSrcRect;

}

SDL_Texture * GameObject::getRenderTexture(SDL_Texture * aTexture)
{
	if (this->definition->isAnimated) {

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
	destRect = this->getRenderDestRect();

	//If this is a primitive, then render a rectangle with the objects primitive color
	//Othwise, render the texture
	if (this->definition->isPrimitive == true)
	{
		game->textureManager.render(&destRect, this->definition->color);
	}
	else
	{
		//Get texture
		texture = this->getRenderTexture(texture);

		//Get render texture src rectangle
		textureSourceRect = this->getRenderTextureRect(textureSourceRect);

		game->textureManager.render(texture, textureSourceRect, &destRect, 0);
	}

	//Loop through any possible child objects, in all 9 positions, and render them too
	if (this->definition->hasChildObjects == true)
	{
		renderChildObjects();
	}


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

	this->removeFromWorld = false;
	this->isChildObject = false;
	this->xSize = definition->xSize;
	this->ySize = definition->ySize;

	//calculate position
	b2Vec2 position(xMapPos * game->worldGridSize.w, yMapPos * game->worldGridSize.h);
	this->setPosition(position, angleAdjust);

	//Get pointer to the texture
	this->texture = game->textureManager.getTexture(definition->textureId);

	//get the animation objects
	if (definition->animations.size() > 0)
	{
		for (auto& animation : definition->animations) {

			//this->animations[animation.second.id] = animation.second;
			this->animations.emplace(animation.second->id, animation.second);

		}

		//On creation, default the animation state to idle
		this->currentAnimationState = "IDLE";
	}

//	buildChildren();

}

void GameObject::setPosition(b2Vec2 position, float angle)
{
	this->xPos = position.x;
	this->yPos = position.y;
	this->angle = angle;
}


void GameObject::addWeapon(string bulletGameObjectId, float xWeaponOffsetPct, float yWeaponOffsetPct)
{


}

b2Vec2 GameObject::calcChildPosition(
	b2Vec2 childSize, 
	int locationSlot,
	int childNumber,
	int childCount,
	float padding,
	bool absolutePositioning, 
	SDL_Rect parentPosition)
{
	b2Vec2 childPosition;
	float x, y;

	//Calculate center of parent
	b2Vec2 parentCenter;
	x = parentPosition.x + (parentPosition.w / 2) -(childSize.x/2);
	y = parentPosition.y + (parentPosition.h / 2) -(childSize.y/2 );
	parentCenter.Set(x,y);

	//Different calcs for the different 9 possible positions
	switch(locationSlot){
		case 1:
			x = parentPosition.x - childSize.x;
			y = parentPosition.y - childSize.y;
			break;
		case 2:
			x = parentCenter.x;
			y = parentPosition.y - childSize.y;
			break;
		case 3:
			x = parentPosition.x + parentPosition.w;
			y = parentPosition.y - childSize.y;
			break;
		case 4:
			x = parentPosition.x - childSize.x;
			y = parentCenter.y;
			break;
		case 5:
			x = parentCenter.x;
			y = parentCenter.y;
			break;
		case 6:
			x = parentPosition.x + parentPosition.w;
			y = parentCenter.y;
			break;
		case 7:
			x = parentPosition.x - childSize.x;
			y = parentPosition.y + parentPosition.h;
			break;
		case 8:
			x = parentCenter.x;
			y = parentPosition.y + parentPosition.h;
			break;
		case 9:
			x = parentPosition.x + parentPosition.w;
			y = parentPosition.y + parentPosition.h;
			break;

	}

	childPosition.x = x;
	childPosition.y = y;

	//Adjust the position if there are multiple children in the same position
	if (childCount > 1)
	{
		float oddEvenadjustValue = 0;
		int stepCount = 0;
		b2Vec2 firstChildPosition;

		//calculate vertical step adjustment depending on even or odd
		if (childCount % 2 == 0)
		{
			//isEvenNumber
			oddEvenadjustValue = (childSize.y + padding) / 2 ;
		}
		else
		{
			oddEvenadjustValue = childSize.y + padding;
		}

		//calculate number of steps to take to place 1st child object
		stepCount = childCount / 2;

		//Calculate 1st child object position based on the previous childPosition calculated
		//values based on location slot
		firstChildPosition.x = childPosition.x;
		firstChildPosition.y = childPosition.y - oddEvenadjustValue - ((childSize.y + padding) * stepCount);

		//Calculate our current child object position using the stepSize and the
		//position of the first child position
		x = firstChildPosition.x;
		y = firstChildPosition.y + ((childSize.y+padding) * childNumber);

	}

	childPosition.x = x;
	childPosition.y = y;


	return childPosition;
	
}

void GameObject::updateChildObjects()
{
	short locationSlot = 0;
	b2Vec2 newChildPosition, childSize;
	SDL_Rect parentPositionRect, childPositionRect;

	for (auto& childLocations : this->childObjects)
	{
		locationSlot++;
		int childNumber = 0;

		if (this->definition->id.compare("GUIPausePanel") == 0)
		{
			int todd = 1;
		}


		for (auto& childObject : childLocations)
		{
			childNumber++;
			int childCount = childLocations.size();
			parentPositionRect = this->getRenderDestRect();
			childSize.Set(childObject->xSize, childObject->ySize);

			//TODO: should be able to pass in the number of children in this position and what number in line
			// this child is into calcChildPosition
			newChildPosition = 
				childObject->calcChildPosition(	
					childSize,		
					locationSlot,
					childNumber,
					childCount,
					this->definition->childPadding,
					false, 
					parentPositionRect);

			childObject->setPosition(newChildPosition, 0);

			childObject->update();
		}
	}

}


void GameObject::renderChildObjects()
{
	//Loop through any possible child objects, in all 9 positions, and render them too
	for (auto& childPositions : this->childObjects)
	{
		for (auto& gameObject : childPositions)
		{
			gameObject->render();
		}
	}

}

void GameObject::buildChildren()
{
	/*
	Loop through all of the child object definitions defined for this GameObject,
	build them and put them in the GameObjects child collection
	*/
	for (ChildObjectDetails childDefinition : this->definition->childObjectDefinitions)
	{
		string childObjectId = childDefinition.gameObjectId;
		int position = childDefinition.position;

		GameObjectDefinition* definition = game->gameObjectManager.getDefinition(childObjectId);

		if (definition != NULL)
		{
			//Build the proper type of GameObject
			if (definition->type.compare("TEXT_OBJECT") == 0)
			{
				TextObject* textObject =
					game->gameObjectManager.buildGameObject<TextObject>(childObjectId, 2, 2, 0);
				textObject->isChildObject = true;
				this->childObjects[position - 1].push_back(make_shared<TextObject>(*textObject));

			}
			else if (definition->type.compare("WORLD_OBJECT") == 0)
			{
				WorldObject* worldObject =
					game->gameObjectManager.buildGameObject<WorldObject>(childObjectId, -5, -5, 0);
				worldObject->isChildObject = true;
				this->childObjects[position - 1].push_back(make_shared<WorldObject>(*worldObject));
			}
			else //default to GAME_OBJECT
			{

				GameObject* gameObject =
					game->gameObjectManager.buildGameObject<GameObject>(childObjectId, -5, -5, 0);
				gameObject->isChildObject = true;
				this->childObjects[position - 1].push_back(make_shared<GameObject>(*gameObject));
			}

		}
	}

}




GameObject::~GameObject()
{

	this->animations.clear();


}

void GameObject::addChildObject(GameObject* childObject, short position)
{

	//this->childObjects[__int64(position)-1].push_back(childObject);

}







