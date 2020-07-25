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

		//Get the Enum that represents the Game Objects action as an int
		int action = EnumMap::instance().toEnum(itrAction["actionId"].asString());

		//Get the actual action Class object and put it in the map for this particular gameObject action
		//i.e. This objects "USE" actionClass could be "FireBullet" or it could be "HealPlayer"
		m_actionMap.emplace(action, JsonToActionClass::instance().toClass(stringActionClass));

	}

	auto noAction = EnumMap::instance().toEnum("ACTION_NONE");
	m_actionMap.emplace(noAction, JsonToActionClass::instance().toClass("NoAction"));
}

ActionComponent::~ActionComponent()
{

}

void ActionComponent::update(std::shared_ptr<GameObject>gameObject)
{


}

void ActionComponent::addAction(std::shared_ptr<Action> action)
{
}


std::shared_ptr<Action> ActionComponent::getAction(size_t actionId)
{

	if (m_actionMap[actionId])
	{
		return m_actionMap[actionId];
	}
	else
	{
		return m_actionMap[ACTION_NONE];
	}

}





