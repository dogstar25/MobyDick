#include "ChildrenComponent.h"

#include "../GameObjectManager.h"

ChildrenComponent::ChildrenComponent()
{

}

ChildrenComponent::ChildrenComponent(std::string gameObjectId)
{
	Json::Value itrJSON = GameObjectManager::instance().getDefinition(gameObjectId)->definitionJSON();

	//Transform Component
	if (itrJSON.isMember("ChildrenComponent"))
	{

		//m_componentFlags |= TRANSFORM_COMPONENT;

	}

}

ChildrenComponent::~ChildrenComponent()
{

}

void ChildrenComponent::update()
{

}

//
//
//b2Vec2 ChildrenComponent::matchParentRotation(SDL_FRect childPositionRect, SDL_FRect parentPositionRect, float parentAngle)
//{
//	b2Vec2 adjustment;
//
//	//calculate parent center
//	b2Vec2 parentCenter(parentPositionRect.x + (parentPositionRect.w / 2), parentPositionRect.y + (parentPositionRect.h / 2));
//
//	//calculate child center
//	b2Vec2 childCenter(childPositionRect.x + (childPositionRect.w / 2), childPositionRect.y + (childPositionRect.h / 2));
//
//	//calculate radius of circle defined by parent and initial child position
//	//This is the hypotenus
//	float radius = 0;
//	radius = sqrt(powf((childCenter.x - parentCenter.x), 2) + powf((childCenter.y - parentCenter.y), 2));
//
//	//calculate the angle of where child is at
//	float y = childCenter.y - parentCenter.y;
//	float x = childCenter.x - parentCenter.x;
//	float childAngle = atan2(childCenter.y - parentCenter.y, childCenter.x - parentCenter.x);
//
//	float childAngleDegrees = util::radiansToDegrees(childAngle);
//
//	//add parent angle
//	float newAngle = childAngle + parentAngle;
//	b2Vec2 newChildPosition{};
//	newChildPosition.x = (radius * cos(newAngle));
//	newChildPosition.y = (radius * sin(newAngle));
//
//	newChildPosition.x += parentCenter.x;
//	newChildPosition.y += parentCenter.y;
//
//	newChildPosition.x -= (childPositionRect.w / 2);
//	newChildPosition.y -= (childPositionRect.h / 2);
//
//	adjustment.x = newChildPosition.x - childPositionRect.x;
//	adjustment.y = newChildPosition.y - childPositionRect.y;
//
//	return adjustment;
//}
//
//
//void ChildrenComponent::updateChildObjects()
//{
//	short locationSlot = 0;
//	b2Vec2 newChildPosition, childSize;
//	SDL_FRect parentPositionRect, childPositionRect;
//
//	for (auto& childLocations : m_childObjects)
//	{
//		locationSlot++;
//		int childNumber = 0;
//		int childCount = childLocations.size();
//
//		for (auto& childObject : childLocations)
//		{
//			childNumber++;
//
//			//Calculate child position
//			newChildPosition = this->calcChildPosition(childObject, locationSlot, childNumber, childCount);
//
//			// Should this child match the angle of the parent
//			if (m_definition->childPositionRelative == true)
//			{
//				childObject->setPosition(newChildPosition, this->angle());
//
//			}
//			else
//			{
//				childObject->setPosition(newChildPosition);
//			}
//
//			//Since the child is a game object itself, call the update function for it
//			//This acts as a recursive call when you have children objects 
//			//within children objects
//			childObject->update();
//		}
//	}
//
//}
//
//
//void ChildrenComponent::renderChildObjects()
//{
//	//Loop through any possible child objects, in all 9 positions, and render them too
//	for (auto& childPositions : m_childObjects)
//	{
//		for (auto& gameObject : childPositions)
//		{
//			gameObject->render();
//		}
//	}
//
//}
//
//void ChildrenComponent::buildChildren()
//{
//	/*
//	Loop through all of the child object definitions defined for this GameObject,
//	build them and put them in the GameObjects child collection
//	*/
//	for (ChildObjectDetails childDefinition : m_definition->childObjectDefinitions)
//	{
//		std::string childObjectId = childDefinition.gameObjectId;
//		unsigned int locationSlot = childDefinition.locationSlot;
//
//		GameObjectDefinition* definition = GameObjectManager::instance().getDefinition(childObjectId);
//
//		if (definition != NULL)
//		{
//			//Build the proper type of GameObject
//			if (definition->type.compare("TEXT_OBJECT") == 0)
//			{
//				TextObject* textObject =
//					GameObjectManager::instance().buildGameObject<TextObject>(childObjectId, 2, 2, 0);
//				m_childObjects[locationSlot - 1].push_back(std::make_shared<TextObject>(*textObject));
//
//			}
//			else if (definition->type.compare("WORLD_OBJECT") == 0)
//			{
//				WorldObject* worldObject =
//					GameObjectManager::instance().buildGameObject<WorldObject>(childObjectId, -5, -5, 0);
//				m_childObjects[locationSlot - 1].push_back(std::make_shared<WorldObject>(*worldObject));
//			}
//			else //default to GAME_OBJECT
//			{
//
//				GameObject* gameObject =
//					GameObjectManager::instance().buildGameObject<GameObject>(childObjectId, -5, -5, 0);
//				m_childObjects[locationSlot - 1].push_back(std::make_shared<GameObject>(*gameObject));
//			}
//
//		}
//	}
//
//}
//
//b2Vec2 ChildrenComponent::calcChildPosition(
//	std::shared_ptr<GameObject> child,
//	int locationSlot,
//	int childNumber,
//	int childCount)
//{
//	SDL_FRect childSize = { child->size().x, child->size().y };
//	SDL_FRect childPositionRect{};
//	float x, y, xAdj = 0, yAdj = 0;
//
//	SDL_FRect parentPositionRect = this->getPositionRect();
//
//	//Calculate center of parent
//	b2Vec2 parentCenter;
//	x = parentPositionRect.x + (parentPositionRect.w / 2);
//	y = parentPositionRect.y + (parentPositionRect.h / 2);
//	parentCenter.Set(x, y);
//
//	//Different calcs for the different 9 possible positions
//	switch (locationSlot) {
//	case 1:
//		x = parentPositionRect.x - childSize.x;
//		y = parentPositionRect.y - childSize.y;
//		break;
//	case 2:
//		x = parentCenter.x - (childSize.x / 2);
//		y = parentPositionRect.y - childSize.y;
//		break;
//	case 3:
//		x = parentPositionRect.x + parentPositionRect.w;
//		y = parentPositionRect.y - childSize.y;
//		break;
//	case 4:
//		x = parentPositionRect.x - childSize.x;
//		y = parentCenter.y - (childSize.y / 2);
//		break;
//	case 5:
//		x = parentCenter.x - (childSize.x / 2);
//		y = parentCenter.y - (childSize.y / 2);
//		break;
//	case 6:
//		x = parentPositionRect.x + parentPositionRect.w;
//		y = parentCenter.y - (childSize.y / 2);
//		break;
//	case 7:
//		x = parentPositionRect.x - childSize.x;
//		y = parentPositionRect.y + parentPositionRect.h;
//		break;
//	case 8:
//		x = parentCenter.x - (childSize.x / 2);
//		y = parentPositionRect.y + parentPositionRect.h;
//		break;
//	case 9:
//		x = parentPositionRect.x + parentPositionRect.w;
//		y = parentPositionRect.y + parentPositionRect.h;
//		break;
//
//	}
//
//	childPositionRect.x = x;
//	childPositionRect.y = y;
//	childPositionRect.w = childSize.x;
//	childPositionRect.h = childSize.y;
//
//	//Adjust the position if there are multiple children in the same position
//	if (childCount > 1)
//	{
//		float oddEvenadjustValue = 0;
//		int stepCount = 0;
//		b2Vec2 firstChildPosition;
//
//		//calculate vertical step adjustment depending on even or odd
//		if (childCount % 2 == 0)
//		{
//			//isEvenNumber
//			oddEvenadjustValue = (childSize.y + m_definition->childPadding) / 2;
//		}
//		else
//		{
//			oddEvenadjustValue = childSize.y + m_definition->childPadding;
//		}
//
//		//calculate number of steps to take to place 1st child object
//		stepCount = childCount / 2;
//
//		//Calculate 1st child object position based on the previous childPosition calculated
//		//values based on location slot
//		firstChildPosition.x =
//			childPositionRect.x;
//		firstChildPosition.y =
//			childPositionRect.y -
//			oddEvenadjustValue -
//			((childSize.y + m_definition->childPadding) * stepCount);
//
//		//Calculate our current child object position using the stepSize and the
//		//position of the first child position
//		childPositionRect.x = firstChildPosition.x;
//		childPositionRect.y =
//			firstChildPosition.y + ((childSize.y + m_definition->childPadding) * childNumber);
//
//
//	}
//
//	if (m_definition->childPositionRelative == true)
//	{
//		b2Vec2 adjustment{};
//
//		adjustment = this->matchParentRotation(
//			childPositionRect,
//			parentPositionRect,
//			this->angle());
//
//		childPositionRect.x += adjustment.x;
//		childPositionRect.y += adjustment.y;
//
//	}
//
//	b2Vec2 childPosition{};
//	childPosition.x = childPositionRect.x;
//	childPosition.y = childPositionRect.y;
//
//
//	return childPosition;
//
//}
//
//
