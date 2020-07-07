#include "ChildrenComponent.h"
#include "../GameObject.h"
#include "../GameObjectManager.h"
#include "../GameConfig.h"

ChildrenComponent::ChildrenComponent()
{

}

ChildrenComponent::ChildrenComponent(std::string gameObjectId, Json::Value definitionJSON)
{

	m_gameObjectId = gameObjectId;

	Json::Value childrenComponentJSON = definitionJSON["childrenComponent"];

	m_childPadding = childrenComponentJSON["childPadding"].asFloat();
	m_childPositionRelative = childrenComponentJSON["childPositionRelative"].asBool();

	for (Json::Value itrChild : childrenComponentJSON["childObjects"])
	{
		std::string childObjectId = itrChild["gameObjectId"].asString();
		int locationSlot = itrChild["locationSlot"].asInt()-1;

		m_childObjects[locationSlot].emplace_back(std::make_shared<GameObject>(childObjectId, -5, -5, 0));

	}



}

ChildrenComponent::~ChildrenComponent()
{

}

void ChildrenComponent::setDependencyReferences(
	std::shared_ptr<TransformComponent> transformComponent)
{

	m_refTransformComponent = transformComponent;

}

void ChildrenComponent::update()
{

	short locationSlot = 0;
	
	for (auto& childLocations : m_childObjects)
	{
		locationSlot++;
		int childNumber = 0;
		int childCount = childLocations.size();
	
		for (auto& childObject : childLocations)
		{
			childNumber++;
	
			//Calculate child position
			b2Vec2 newChildPosition = _calcChildPosition(childObject->transformComponent()->size(), locationSlot, childNumber, childCount);
	
			// Should this child match the angle of the parent
			if (m_childPositionRelative == true)
			{
				childObject->setPosition(newChildPosition, m_refTransformComponent->angle());
	
			}
			else
			{
				childObject->setPosition(newChildPosition, -1);
			}
	
			//Since the child is a game object itself, call the update function for it
			//This acts as a recursive call when you have children objects 
			//within children objects
			childObject->update();
		}
	}
	
}


b2Vec2 ChildrenComponent::matchParentRotation(SDL_FRect childPositionRect, SDL_FRect parentPositionRect, float parentAngle)
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

	//add parent angle
	float newAngle = childAngle + util::degreesToRadians(parentAngle);
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

void ChildrenComponent::renderChildren()
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
//


b2Vec2 ChildrenComponent::_calcChildPosition(
	b2Vec2 childSize,
	int locationSlot,
	int childNumber,
	int childCount)
{
	//SDL_FRect childSize = { child->size().x, child->size().y };
	SDL_FRect childPositionRect{};
	float x, y, xAdj = 0, yAdj = 0;

	//Parent position is center of parent
	SDL_FRect parentPositionRect = m_refTransformComponent->getPositionRect();

	//Different calcs for the different 9 possible positions
	//Calculate topleft corner of child
	switch (locationSlot) {
	case 1:
		x = parentPositionRect.x - (childSize.x);
		y = parentPositionRect.y - (childSize.y);
		break;
	case 2:
		x = parentPositionRect.x;
		y = parentPositionRect.y - (childSize.y);
		break;
	case 3:
		x = parentPositionRect.x + childSize.x;
		y = parentPositionRect.y - (childSize.y);
		break;
	case 4:
		x = parentPositionRect.x - (childSize.x);
		y = parentPositionRect.y;
		break;
	case 5:
		x = parentPositionRect.x;
		y = parentPositionRect.y;
		break;
	case 6:
		x = parentPositionRect.x + childSize.x;
		y = parentPositionRect.y;
		break;
	case 7:
		x = parentPositionRect.x - (childSize.x);
		y = parentPositionRect.y + childSize.y;
		break;
	case 8:
		x = parentPositionRect.x;
		y = parentPositionRect.y + childSize.y;
		break;
	case 9:
		x = parentPositionRect.x + childSize.x;
		y = parentPositionRect.y + childSize.y;
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
			oddEvenadjustValue = (childSize.y + m_childPadding) / 2;
		}
		else
		{
			oddEvenadjustValue = childSize.y + m_childPadding;
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
			((childSize.y + m_childPadding) * stepCount);

		//Calculate our current child object position using the stepSize and the
		//position of the first child position
		childPositionRect.x = firstChildPosition.x;
		childPositionRect.y =
			firstChildPosition.y + ((childSize.y + m_childPadding) * childNumber);


	}

	if (m_childPositionRelative == true)
	{
		b2Vec2 adjustment{};

		adjustment = this->matchParentRotation(
			childPositionRect,
			parentPositionRect,
			m_refTransformComponent->angle());

		childPositionRect.x += adjustment.x;
		childPositionRect.y += adjustment.y;

	}

	b2Vec2 childPosition{};
	childPosition.x = childPositionRect.x;
	childPosition.y = childPositionRect.y;


	return childPosition;

}

