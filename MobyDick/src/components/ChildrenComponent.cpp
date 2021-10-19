#include "ChildrenComponent.h"
#include "../GameObject.h"
#include "../Scene.h"

ChildrenComponent::ChildrenComponent()
{

}

ChildrenComponent::ChildrenComponent(Json::Value definitionJSON, Scene* parentScene)
{

	Json::Value childrenComponentJSON = definitionJSON["childrenComponent"];

	m_childPadding = childrenComponentJSON["childPadding"].asFloat();
	m_childPositionRelative = childrenComponentJSON["childPositionRelative"].asBool();

	for (Json::Value itrChild : childrenComponentJSON["childObjects"])
	{
		std::string childObjectId = itrChild["gameObjectId"].asString();
		int locationSlot = itrChild["locationSlot"].asInt()-1;

		m_childObjects[locationSlot].emplace_back(std::make_shared<GameObject>(childObjectId, -1.0F, -1.0F, 0.F, parentScene, LAYER_MAIN));

	}
}

ChildrenComponent::~ChildrenComponent()
{



}


void ChildrenComponent::update()
{

	short locationSlot = 0;
	
	for (auto& childLocations : m_childObjects)
	{
		locationSlot++;
		int childNumber = 0;
		int childCount = (int)childLocations.size();
	
		for (auto& childObject : childLocations)
		{
			const auto& transformComponent = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
			const auto& childTransformComponent = childObject->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

			childNumber++;
	
			//Calculate child position
			SDL_FPoint parentCenterPosition = transformComponent->getCenterPosition();
			float parentAngle = transformComponent->angle();
			b2Vec2 newChildPosition = 
				_calcChildPosition(childTransformComponent->size(), locationSlot, childNumber, childCount, parentCenterPosition, parentAngle);
	
			// Should this child match the angle of the parent
			if (m_childPositionRelative == true)
			{
				childObject->setPosition(newChildPosition, transformComponent->angle());
	
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


b2Vec2 ChildrenComponent::_calcChildPosition(
	b2Vec2 childSize,
	int locationSlot,
	int childNumber,
	int childCount,
	SDL_FPoint parentCenterPosition,
	float parentAngle)
{
	//SDL_FRect childSize = { child->size().x, child->size().y };
	//SDL_FPoint childPosition{};
	float x=0, y=0, xAdj = 0, yAdj = 0;

	//Different calculations for the different 9 possible positions
	//Calculate top left corner of child
	switch (locationSlot) {
	case 1:
		x = parentCenterPosition.x - (childSize.x);
		y = parentCenterPosition.y - (childSize.y);
		break;
	case 2:
		x = parentCenterPosition.x;
		y = parentCenterPosition.y - (childSize.y);
		break;
	case 3:
		x = parentCenterPosition.x + childSize.x;
		y = parentCenterPosition.y - (childSize.y);
		break;
	case 4:
		x = parentCenterPosition.x - (childSize.x);
		y = parentCenterPosition.y;
		break;
	case 5:
		x = parentCenterPosition.x;
		y = parentCenterPosition.y;
		break;
	case 6:
		x = parentCenterPosition.x + childSize.x;
		y = parentCenterPosition.y;
		break;
	case 7:
		x = parentCenterPosition.x - (childSize.x);
		y = parentCenterPosition.y + childSize.y;
		break;
	case 8:
		x = parentCenterPosition.x;
		y = parentCenterPosition.y + childSize.y;
		break;
	case 9:
		x = parentCenterPosition.x + childSize.x;
		y = parentCenterPosition.y + childSize.y;
		break;

	}

	SDL_FPoint childCenterPosition{ x, y };

	//Adjust the position if there are multiple children in the same position
	if (childCount > 1)
	{
		float oddEvenadjustValue = 0;
		int stepCount = 0;
		b2Vec2 firstChildPosition = {};

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
		firstChildPosition.x = childCenterPosition.x;
		firstChildPosition.y = 
			childCenterPosition.y -
			oddEvenadjustValue -
			((childSize.y + m_childPadding) * stepCount);

		//Calculate our current child object position using the stepSize and the
		//position of the first child position
		childCenterPosition.x = firstChildPosition.x;
		childCenterPosition.y =
			firstChildPosition.y + ((childSize.y + m_childPadding) * childNumber);

	}

	if (m_childPositionRelative == true)
	{
		b2Vec2 adjustment{};

		adjustment = util::matchParentRotation(childCenterPosition, parentCenterPosition, parentAngle);

		childCenterPosition.x += adjustment.x;
		childCenterPosition.y += adjustment.y;

	}

	b2Vec2 b2Vec2ChildPosition = { childCenterPosition.x, childCenterPosition.y };
	return b2Vec2ChildPosition;

}


