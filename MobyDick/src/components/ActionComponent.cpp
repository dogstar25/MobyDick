#include "ActionComponent.h"
#include "../actions/HeroFireAction.h"
#include "../JsonToActionClass.h"
#include "../EnumMaps.h"

#include "../ecs/Types.hpp"

ActionComponent::ActionComponent(Json::Value definitionJSON)
{
	Json::Value componentJSON = definitionJSON["actionComponent"];

	for (Json::Value itrAction: componentJSON["actions"])
	{
		//Get the name of the class to be used as the action as a string
		std::string stringActionClass = itrAction["actionClass"].asString();

		//Get the Enum that reprsents the Game Objects action as an int
		int action = EnumMap::instance().toEnum(itrAction["actionId"].asString());

		//Get the actual action Class object and put it in the map for this particular gameObject action
		//i.e. This objects "USE" actionClass could be "FireBullet" or it could be "HealPlayer"
		m_actionMap.emplace(action, JsonToActionClass::instance().toClass(stringActionClass));

	}
}

void ActionComponent::moveAction(Entity entity, int direction, int strafe)
{
	m_actionMap[ACTION_MOVE]->perform(entity, direction, strafe);
}

void ActionComponent::rotateAction(Entity entity, float angularVelocity)
{
	m_actionMap[ACTION_ROTATE]->perform(entity, angularVelocity);
}

void ActionComponent::useAction(Entity entity)
{
	m_actionMap[ACTION_USE]->perform(entity);
}




