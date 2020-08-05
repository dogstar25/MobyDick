#include "ActionComponent.h"
#include "../GameObject.h"
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

		if (action == ACTION_MOVE) {
			m_moveAction = JsonToActionClass::instance().toMoveClass(stringActionClass);
		}
		else if (action == ACTION_ROTATE) {
			m_rotateAction = JsonToActionClass::instance().toRotateClass(stringActionClass);
		}
		else if (action == ACTION_USE) {
			m_useAction = JsonToActionClass::instance().toUseClass(stringActionClass);
		}
		else if (action == ACTION_INTERACT) {
			m_interactAction = JsonToActionClass::instance().toInteractClass(stringActionClass);
		}

	}

}

ActionComponent::~ActionComponent()
{

}

void ActionComponent::update()
{


}


void ActionComponent::perform(MoveAction action)
{
	m_moveAction->setDirection(action.direction());
	m_moveAction->setStrafe(action.strafe());
	m_moveAction->perform(m_parentGameObject);
}

void ActionComponent::perform(RotateAction action)
{
	m_rotateAction->perform(m_parentGameObject);
}

void ActionComponent::perform(UseAction action)
{
	m_useAction->perform(m_parentGameObject);
}

void ActionComponent::perform(InteractAction action)
{
	m_interactAction->perform(m_parentGameObject);
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





