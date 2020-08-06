#include "ActionComponent.h"
#include "../GameObject.h"
#include "../actions/ActionMaps.h"
#include "../EnumMaps.h"

ActionComponent::ActionComponent(Json::Value definitionJSON)
{
	Json::Value componentJSON = definitionJSON["actionComponent"];

	for (Json::Value itrAction: componentJSON["actions"])
	{
		//Get the name of the class to be used as the action as a string
		std::string actionCode = itrAction["actionClass"].asString();

		//Get the Enum that represents the Game Objects action as an int
		int actionId = EnumMap::instance().toEnum(itrAction["actionId"].asString());

		m_actions.emplace(actionId, actionCode);

	}

}

ActionComponent::~ActionComponent()
{

}

void ActionComponent::update()
{


}

std::shared_ptr<MoveAction> ActionComponent::buildMoveAction(int direction, int strafe) {

	std::string actionId = m_actions.at(ACTION_MOVE);
	std::shared_ptr<MoveAction> action = ActionMaps::instance().getMoveAction(actionId);
	action->setDirection(direction);
	action->setStrafe(strafe);

	return action;
}

std::shared_ptr<RotateAction> ActionComponent::buildRotateAction(float angularVelocity) {

	std::string actionId = m_actions.at(ACTION_ROTATE);
	std::shared_ptr<RotateAction> action = ActionMaps::instance().getRotateAction(actionId);
	action->setAngularVelocity(angularVelocity);

	return action;
}

std::shared_ptr<UseAction> ActionComponent::buildUseAction() {

	std::string actionId = m_actions.at(ACTION_USE);
	std::shared_ptr<UseAction> action = ActionMaps::instance().getUseAction(actionId);

	return action;
}

std::shared_ptr<InteractAction> ActionComponent::buildInteractAction() {

	std::string actionId = m_actions.at(ACTION_INTERACT);
	std::shared_ptr<InteractAction> action = ActionMaps::instance().getInteractAction(actionId);

	return action;
}

void ActionComponent::performMoveAction(int direction, int strafe)
{
	buildMoveAction(direction, strafe)->perform(m_parentGameObject);
}

void ActionComponent::performRotateAction(float angularVelocity)
{
	buildRotateAction(angularVelocity)->perform(m_parentGameObject);
}

void ActionComponent::performUseAction()
{
	buildUseAction()->perform(m_parentGameObject);
}

void ActionComponent::performInteractAction()
{
	buildInteractAction()->perform(m_parentGameObject);
}





