#include "ActionComponent.h"
#include "../GameObject.h"
#include "../actions/HeroFireAction.h"
#include "../JsonToActionClass.h"
#include "../EnumMaps.h"

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

ActionComponent::~ActionComponent()
{

}

void ActionComponent::moveAction(GameObject* gameObject, int direction, int strafe)
{

	m_actionMap[ACTION_MOVE]->perform(gameObject, direction, strafe);

}

void ActionComponent::rotateAction(GameObject* gameObject, float angularVelocity)
{

	m_actionMap[ACTION_ROTATE]->perform(gameObject, angularVelocity);
}

void ActionComponent::useAction(GameObject* gameObject)
{
	m_actionMap[ACTION_USE]->perform(gameObject);
}




