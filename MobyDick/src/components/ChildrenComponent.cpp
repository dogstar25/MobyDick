#include "ChildrenComponent.h"
#include "../GameObject.h"

ChildrenComponent::ChildrenComponent()
{

}

ChildrenComponent::ChildrenComponent(Json::Value definitionJSON)
{

	Json::Value childrenComponentJSON = definitionJSON["childrenComponent"];

	m_childPadding = childrenComponentJSON["childPadding"].asFloat();
	m_childPositionRelative = childrenComponentJSON["childPositionRelative"].asBool();

	for (Json::Value itrChild : childrenComponentJSON["childObjects"])
	{
		std::string childObjectId = itrChild["gameObjectId"].asString();
		int locationSlot = itrChild["locationSlot"].asInt()-1;

		m_childObjects[locationSlot].emplace_back(std::make_shared<GameObject>(childObjectId, -1.0F, -1.0F, 0.F))->init();

	}
}

ChildrenComponent::~ChildrenComponent()
{



}


void ChildrenComponent::update()
{
	const auto& transformComponent = parent()->getComponent<TransformComponent>();

	short locationSlot = 0;
	
	for (auto& childLocations : m_childObjects)
	{
		locationSlot++;
		int childNumber = 0;
		int childCount = (int)childLocations.size();
	
		for (auto& childObject : childLocations)
		{
			//const auto& childTransformComponent = childObject->getComponent<TransformComponent>();

			childNumber++;
	
			//Calculate child position
			//SDL_FPoint parentPosition = transformComponent->getCenterPosition();
			sf::Vector2f parentPosition = parent()->getPosition();
			float parentAngle = parent()->getRotation();
			sf::Vector2f newChildPosition =
				_calcChildPosition(parent()->size(), locationSlot, childNumber, childCount, parentPosition, parentAngle);
	
			// Should this child match the angle of the parent
			if (m_childPositionRelative == true)
			{
				childObject->setPosition(newChildPosition, parent()->getRotation());
	
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


sf::Vector2f ChildrenComponent::_calcChildPosition(
	sf::Vector2f childSize,
	int locationSlot,
	int childNumber,
	int childCount,
	sf::Vector2f parentPosition,
	float parentAngle)
{
	//SDL_FRect childSize = { child->size().x, child->size().y };
	//SDL_FPoint childPosition{};
	float x=0, y=0, xAdj = 0, yAdj = 0;

	//Parent position is center of parent
	//SDL_FRect parentPositionRect = parentPositionRec;
//	SDL_FRect parentPositionRect = m_gameObject->transformComponent()->getPositionRect();

	//Different calcs for the different 9 possible positions
	//Calculate topleft corner of child
	switch (locationSlot) {
	case 1:
		x = parentPosition.x - (childSize.x);
		y = parentPosition.y - (childSize.y);
		break;
	case 2:
		x = parentPosition.x;
		y = parentPosition.y - (childSize.y);
		break;
	case 3:
		x = parentPosition.x + childSize.x;
		y = parentPosition.y - (childSize.y);
		break;
	case 4:
		x = parentPosition.x - (childSize.x);
		y = parentPosition.y;
		break;
	case 5:
		x = parentPosition.x;
		y = parentPosition.y;
		break;
	case 6:
		x = parentPosition.x + childSize.x;
		y = parentPosition.y;
		break;
	case 7:
		x = parentPosition.x - (childSize.x);
		y = parentPosition.y + childSize.y;
		break;
	case 8:
		x = parentPosition.x;
		y = parentPosition.y + childSize.y;
		break;
	case 9:
		x = parentPosition.x + childSize.x;
		y = parentPosition.y + childSize.y;
		break;

	}

	sf::Vector2f childPosition{ x, y };

	//Adjust the position if there are multiple children in the same position
	if (childCount > 1)
	{
		float oddEvenadjustValue = 0;
		int stepCount = 0;
		sf::Vector2f firstChildPosition = {};

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
		firstChildPosition.x = childPosition.x;
		firstChildPosition.y = 
			childPosition.y -
			oddEvenadjustValue -
			((childSize.y + m_childPadding) * stepCount);

		//Calculate our current child object position using the stepSize and the
		//position of the first child position
		childPosition.x = firstChildPosition.x;
		childPosition.y =
			firstChildPosition.y + ((childSize.y + m_childPadding) * childNumber);

	}

	if (m_childPositionRelative == true)
	{
		sf::Vector2f adjustment{};

		adjustment = this->_matchParentRotation(childPosition, parentPosition, parentAngle);

		childPosition.x += adjustment.x;
		childPosition.y += adjustment.y;

	}

	//Need to convert to a b2vec2
	//ToDo: come up with our own vec2, etc class
	sf::Vector2f b2Vec2ChildPosition = { childPosition.x, childPosition.y };
	return b2Vec2ChildPosition;

}

sf::Vector2f ChildrenComponent::_matchParentRotation(sf::Vector2f childPosition, sf::Vector2f parentPosition, float parentAngle)
{
	sf::Vector2f adjustment;

	//calculate radius of circle defined by parent and initial child position
	//This is the hypotenus
	float radius = 0;
	radius = sqrt(powf((childPosition.x - parentPosition.x), 2) + powf((childPosition.y - parentPosition.y), 2));

	//calculate the angle of where child is at
	float y = childPosition.y - parentPosition.y;
	float x = childPosition.x - parentPosition.x;
	float childAngle = atan2(childPosition.y - parentPosition.y, childPosition.x - parentPosition.x);

	//add parent angle
	float newAngle = childAngle + util::degreesToRadians(parentAngle);
	sf::Vector2f newChildPosition{};
	newChildPosition.x = (radius * cos(newAngle));
	newChildPosition.y = (radius * sin(newAngle));

	newChildPosition.x += parentPosition.x;
	newChildPosition.y += parentPosition.y;

	adjustment.x = newChildPosition.x - childPosition.x;
	adjustment.y = newChildPosition.y - childPosition.y;

	return adjustment;
}

