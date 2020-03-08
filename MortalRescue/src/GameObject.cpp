#include "GameObject.h"
#include "TextObject.h"

#include "Game.h"
#include "GameObjectDefinition.h"
#include "Animation.h"
#include "Texture.h"
#include "TextureManager.h"

#include "WorldObject.h"


GameObject::~GameObject()
{

	m_animations.clear();

	for (int x = 0; x < constants::CHILD_POSITIONS; x++)
	{
		m_childObjects[x].clear();
	}


}

void GameObject::init()
{

	//this->definitionId;
		//this->m_currentAnimationState;

	m_removeFromWorld = false;
	m_color = { 255,255,255,255 };
	m_position = { 0,0 };
	m_size = { 0,0 };
	m_angle = 0;
	m_mouseState = 0;

	m_texture = nullptr;

	m_definition = nullptr;

}

GameObject::GameObject()
{
	this->init();
}

GameObject::GameObject(string gameObjectId, float xMapPos, float yMapPos, float angleAdjust)
{
	//Init
	this->init();

	//calculate position
	b2Vec2 position(xMapPos * game->worldGridSize.w, yMapPos * game->worldGridSize.h);
	this->setPosition(position, angleAdjust);

	//set the id and it will be unique for every game object in the game
	//this->m_id = gameObjectId + to_string(game->gameObjectCount);
	//this->id = gameObjectId;
	m_definitionId = gameObjectId;

	//is this a debug object then get the default debug definition but change its 
	//id value to the one we passed in
	if (gameObjectId.rfind("DEBUG_", 0) == 0)
	{
		m_definition = GameObjectManager::instance().gameObjectDefinitions["DEBUG_ITEM"];;
	}
	else
	{
		m_definition = GameObjectManager::instance().gameObjectDefinitions[gameObjectId];
	}

	m_size.Set(m_definition->xSize, m_definition->ySize);

	//color
	m_color = m_definition->color;

	//Get pointer to the texture
	m_texture = TextureManager::instance().getTexture(m_definition->textureId);

	//get the animation objects if they exist
	string firstState;
	int i = 0;
	if (m_definition->animationDetails.animations.size() > 0)
	{
		Animation* animation = nullptr;
		for (auto animationDefinition : m_definition->animationDetails.animations) {

			animation = new Animation(m_definition,
				animationDefinition.state,
				animationDefinition.textureId,
				animationDefinition.frames,
				animationDefinition.speed);

			m_animations.emplace(animationDefinition.state, animation);
			//this->animations[animationDefinition.state]= animation;

			//Save the first state id 
			if (i == 0) {
				firstState = animationDefinition.state;
			}
			++i;
		}

		//On creation, default the animation state to idle
		this->m_currentAnimationState = firstState;
	}

	//Build children if they exist
	this->buildChildren();

}

void GameObject::update()
{
	//If this object is animated, then animate it
	if(m_definition->isAnimated) {
		m_animations[this->m_currentAnimationState]->animate();
	}

	//Update the mouse state
	if (SDL_GetRelativeMouseMode() == SDL_FALSE)
	{
		this->updateMouseState();
	}

	//This object was clicked, so push whatever event is tied to its onClick event property
	if (m_mouseState == constants::MOUSE_CLICKED)
	{
		this->onMouseClickEvent();
	}

	//Loop through any possible child objects and update their
	// position to reflect parent objects position
	if (m_definition->hasChildObjects == true)
	{
		updateChildObjects();
	}

}

SDL_Rect GameObject::getPositionRect()
{
	SDL_Rect positionRect;

	positionRect.w = m_size.x;
	positionRect.h = m_size.y;

	positionRect.x = m_position.x;
	positionRect.y = m_position.y;

	return positionRect;

}

SDL_Rect GameObject::getRenderDestRect()
{
	SDL_Rect destRect{};

	//Get the position/size rectangle of the object
	destRect = this->getPositionRect();

	//adjust render position X and Y for camera if not an absolute positioned object
	if (m_definition->absolutePositioning == false)
	{
		destRect.x -= Camera::instance().frame().x;
		destRect.y -= Camera::instance().frame().y;
	}

	return destRect;

}

SDL_Rect*  GameObject::getRenderTextureRect()
{
	SDL_Rect* textureSrcRect=NULL;

	if (m_definition->isAnimated) {

		textureSrcRect = m_animations[this->m_currentAnimationState]->getCurrentTextureAnimationSrcRect();
	}

	return textureSrcRect;

}

SDL_Texture * GameObject::getRenderTexture()
{
	SDL_Texture* texture = nullptr;

	if (m_definition->isAnimated) {

		texture = m_animations[this->m_currentAnimationState]->getTexture();
	}
	else {

		texture = m_texture->sdlTexture;
	}

	return texture;

}

void GameObject::render()
{
	SDL_Rect* textureSourceRect=NULL, destRect;
	SDL_Texture* texture=NULL;

	//Get render destination rectangle
	destRect = this->getRenderDestRect();

	//Get texture
	texture = this->getRenderTexture();

	//Get render texture src rectangle
	textureSourceRect = this->getRenderTextureRect();

	//All angles on objects should be in radians to kep consistency with box2d objects
	//it needs to be converted to degrees for SDL to display
	float angle = util::radiansToDegrees(this->angle());
	TextureManager::instance().render(texture, m_color, textureSourceRect, &destRect, angle);

	//test outlining object
	if (m_definition->isMouseSelectable)
	{
		if (m_mouseState == constants::MOUSE_HOVER)
		{
			this->onMouseHoverRender();
		}
		else if (m_mouseState == constants::MOUSE_HOLD)
		{
			this->onMouseHoldRender();
		}
		else if (m_mouseState == constants::MOUSE_CLICKED)
		{
			this->onMouseClickRender();
		}
	}

	//Outline the object if defined
	if (m_definition->renderOutline)
	{
		TextureManager::instance().outLineObject(this, 2);
	}
	
	//Loop through any possible child objects, in all 9 positions, and render them too
	if (m_definition->hasChildObjects == true)
	{
		renderChildObjects();
	}


}


b2Vec2 GameObject::matchParentRotation(SDL_Rect childPositionRect, SDL_Rect parentPositionRect, float parentAngle)
{
	b2Vec2 adjustment;

	//calculate parent center
	b2Vec2 parentCenter(parentPositionRect.x + (parentPositionRect.w / 2), parentPositionRect.y + (parentPositionRect.h / 2));

	//calculate child center
	b2Vec2 childCenter(childPositionRect.x + (childPositionRect.w / 2), childPositionRect.y + (childPositionRect.h / 2));

	//calculate radius of circle defined by parent and initial child position
	//This is the hypotenus
	float radius = 0;
	radius = sqrt(powf((childCenter.x - parentCenter.x), 2) + powf((childCenter.y - parentCenter.y), 2));

	//calculate the angle of where child is at
	float y = childCenter.y - parentCenter.y;
	float x = childCenter.x - parentCenter.x;
	float childAngle = atan2(childCenter.y - parentCenter.y, childCenter.x - parentCenter.x);

	float childAngleDegrees = util::radiansToDegrees(childAngle);

	//add parent angle
	float newAngle = childAngle + parentAngle;
	b2Vec2 newChildPosition{};
	newChildPosition.x = (radius * cos(newAngle));
	newChildPosition.y = (radius * sin(newAngle));

	newChildPosition.x += parentCenter.x;
	newChildPosition.y += parentCenter.y;

	newChildPosition.x -= (childPositionRect.w / 2);
	newChildPosition.y -= (childPositionRect.h / 2);

	adjustment.x = newChildPosition.x - childPositionRect.x;
	adjustment.y = newChildPosition.y - childPositionRect.y;

	return adjustment;
}


void GameObject::updateChildObjects()
{
	short locationSlot = 0;
	b2Vec2 newChildPosition, childSize;
	SDL_Rect parentPositionRect, childPositionRect;

	for (auto& childLocations : m_childObjects)
	{
		locationSlot++;
		int childNumber = 0;
		int childCount = childLocations.size();

		for (auto& childObject : childLocations)
		{
			childNumber++;

			//Calculate child position
			newChildPosition = this->calcChildPosition(childObject, locationSlot, childNumber, childCount);

			// Should this child match the angle of the parent
			if (m_definition->childPositionRelative == true)
			{
				childObject->setPosition(newChildPosition, this->angle());

			}
			else
			{
				childObject->setPosition(newChildPosition);
			}

			//Since the child is a game object itself, call the update function for it
			//This acts as a recursive call when you have children objects 
			//within children objects
			childObject->update();
		}
	}

}


void GameObject::renderChildObjects()
{
	//Loop through any possible child objects, in all 9 positions, and render them too
	for (auto& childPositions : m_childObjects)
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
	for (ChildObjectDetails childDefinition : m_definition->childObjectDefinitions)
	{
		string childObjectId = childDefinition.gameObjectId;
		unsigned int locationSlot = childDefinition.locationSlot;

		GameObjectDefinition* definition = GameObjectManager::instance().getDefinition(childObjectId);

		if (definition != NULL)
		{
			//Build the proper type of GameObject
			if (definition->type.compare("TEXT_OBJECT") == 0)
			{
				TextObject* textObject =
					GameObjectManager::instance().buildGameObject<TextObject>(childObjectId, 2, 2, 0);
				m_childObjects[locationSlot - 1].push_back(make_shared<TextObject>(*textObject));

			}
			else if (definition->type.compare("WORLD_OBJECT") == 0)
			{
				WorldObject* worldObject =
					GameObjectManager::instance().buildGameObject<WorldObject>(childObjectId, -5, -5, 0);
				m_childObjects[locationSlot - 1].push_back(make_shared<WorldObject>(*worldObject));
			}
			else //default to GAME_OBJECT
			{

				GameObject* gameObject =
					GameObjectManager::instance().buildGameObject<GameObject>(childObjectId, -5, -5, 0);
				m_childObjects[locationSlot - 1].push_back(make_shared<GameObject>(*gameObject));
			}

		}
	}

}

b2Vec2 GameObject::calcChildPosition(
	shared_ptr<GameObject> child,
	int locationSlot,
	int childNumber,
	int childCount)
{
	SDL_Rect childSize = { child->size().x, child->size().y };
	SDL_Rect childPositionRect{};
	float x, y, xAdj = 0, yAdj = 0;

	SDL_Rect parentPositionRect = this->getPositionRect();

	//Calculate center of parent
	b2Vec2 parentCenter;
	x = parentPositionRect.x + (parentPositionRect.w / 2);
	y = parentPositionRect.y + (parentPositionRect.h / 2);
	parentCenter.Set(x, y);

	//Different calcs for the different 9 possible positions
	switch (locationSlot) {
	case 1:
		x = parentPositionRect.x - childSize.x;
		y = parentPositionRect.y - childSize.y;
		break;
	case 2:
		x = parentCenter.x - (childSize.x / 2);
		y = parentPositionRect.y - childSize.y;
		break;
	case 3:
		x = parentPositionRect.x + parentPositionRect.w;
		y = parentPositionRect.y - childSize.y;
		break;
	case 4:
		x = parentPositionRect.x - childSize.x;
		y = parentCenter.y - (childSize.y / 2);
		break;
	case 5:
		x = parentCenter.x - (childSize.x / 2);
		y = parentCenter.y - (childSize.y / 2);
		break;
	case 6:
		x = parentPositionRect.x + parentPositionRect.w;
		y = parentCenter.y - (childSize.y / 2);
		break;
	case 7:
		x = parentPositionRect.x - childSize.x;
		y = parentPositionRect.y + parentPositionRect.h;
		break;
	case 8:
		x = parentCenter.x - (childSize.x / 2);
		y = parentPositionRect.y + parentPositionRect.h;
		break;
	case 9:
		x = parentPositionRect.x + parentPositionRect.w;
		y = parentPositionRect.y + parentPositionRect.h;
		break;

	}

	childPositionRect.x = x;
	childPositionRect.y = y;
	childPositionRect.w = childSize.x;
	childPositionRect.h = childSize.y;

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
			oddEvenadjustValue = (childSize.y + m_definition->childPadding) / 2;
		}
		else
		{
			oddEvenadjustValue = childSize.y + m_definition->childPadding;
		}

		//calculate number of steps to take to place 1st child object
		stepCount = childCount / 2;

		//Calculate 1st child object position based on the previous childPosition calculated
		//values based on location slot
		firstChildPosition.x =
			childPositionRect.x;
		firstChildPosition.y =
			childPositionRect.y -
			oddEvenadjustValue -
			((childSize.y + m_definition->childPadding) * stepCount);

		//Calculate our current child object position using the stepSize and the
		//position of the first child position
		childPositionRect.x = firstChildPosition.x;
		childPositionRect.y =
			firstChildPosition.y + ((childSize.y + m_definition->childPadding) * childNumber);


	}

	if (m_definition->childPositionRelative == true)
	{
		b2Vec2 adjustment{};

		adjustment = this->matchParentRotation(
			childPositionRect,
			parentPositionRect,
			this->angle());

		childPositionRect.x += adjustment.x;
		childPositionRect.y += adjustment.y;

	}

	b2Vec2 childPosition{};
	childPosition.x = childPositionRect.x;
	childPosition.y = childPositionRect.y;


	return childPosition;

}



void GameObject::onMouseHoverRender()
{

	TextureManager::instance().outLineObject(this, 2);


}

void GameObject::onMouseClickRender()
{

	TextureManager::instance().outLineObject(this, 6);

}

void GameObject::onMouseHoldRender()
{
	TextureManager::instance().outLineObject(this, 2);
}

void GameObject::onMouseClickEvent()
{
	string* actionCode;

	actionCode = new string(m_definition->onClickAction);
	SDL_Event event;
	event.user.data1 = static_cast<void*>(actionCode);
	event.type = SDL_USEREVENT;
	SDL_PushEvent(&event);
}

void GameObject::updateMouseState()
{
	SDL_Rect gameObjectDrawRect;
	gameObjectDrawRect = this->getRenderDestRect();
	bool isHovered = false;

	if (m_definition->isMouseSelectable == true)
	{
		//Is mouse over the object
		if (game->mouseLocation.x >= gameObjectDrawRect.x &&
			game->mouseLocation.x <= gameObjectDrawRect.x + gameObjectDrawRect.w &&
			game->mouseLocation.y >= gameObjectDrawRect.y &&
			game->mouseLocation.y <= gameObjectDrawRect.y + gameObjectDrawRect.h)
		{

			//was this object clicked?
			if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
			{

				//Was this object already in a hold state, meaning user is holding mouse clicked on object
				if (m_mouseState == constants::MOUSE_HOLD)
				{
					//stay in "hold" state while user is holding click on object
					while (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
					{
						SDL_PumpEvents();
					}

					//User has released mouse so now execute the object onClick event
					//this->onMouseClick();
					m_mouseState = constants::MOUSE_CLICKED;

				}
				else
				{
					m_mouseState = constants::MOUSE_HOLD;
				}

			}
			else
			{
				m_mouseState = constants::MOUSE_HOVER;
			}
		}
		else
		{
			m_mouseState = constants::MOUSE_NONE;
		}
	}

}

void GameObject::setPosition(b2Vec2 position)
{
	m_position = position;
}

void GameObject::setPosition(float xPosition, float yPosition)
{
	m_position.x = xPosition;
	m_position.x = yPosition;
}

void GameObject::setPosition(b2Vec2 position, float angle)
{
	m_position = position;
	m_angle = angle;
}

void GameObject::setPosition(float xPosition, float yPosition, float angle)
{
	m_position.x = xPosition;
	m_position.x = yPosition;
	m_angle = angle;
}

void GameObject::setSize(b2Vec2 size)
{
	m_size = size;
}

void GameObject::setSize(float xSize, float ySize)
{
	m_size.x = xSize;
	m_size.y = ySize;
}

void GameObject::setAngle(float angle)
{
	m_angle = angle;
}

void GameObject::setColor(int red, int green, int blue, int alpha)
{
	m_color.r = red;
	m_color.g = green;
	m_color.b = blue;
	m_color.a = alpha;

}