#include "ActionComponent.h"

#include <assert.h>

#include "../GameObject.h"
#include "../actions/ActionMaps.h"
#include "../EnumMaps.h"

#include "../actions/MoveAction.h"

ActionComponent::ActionComponent(Json::Value definitionJSON)
{
	Json::Value componentJSON = definitionJSON["actionComponent"];

	for (Json::Value itrAction: componentJSON["actions"])
	{
		//Get the name of the class to be used as the action as a string
		std::string actionCode = itrAction["actionClass"].asString();

		//Get the Enum that represents the Game Objects action as an int
		int actionId = EnumMap::instance().toEnum(itrAction["actionId"].asString());

		m_actions[actionId] = actionCode;

	}

}

ActionComponent::~ActionComponent()
{

}

void ActionComponent::update()
{


}

std::shared_ptr<MoveAction> ActionComponent::buildMoveAction(int direction, int strafe) 
{

	std::shared_ptr<MoveAction> action;

	//Get the action if it exists, otherwise we will use the default actionClass for this action type
	std::optional<std::string> actionKey = _getActionKey(ACTION_MOVE);

	if (actionKey) {
		action = std::dynamic_pointer_cast<MoveAction>(ActionMaps::instance().getAction(actionKey->data()));
	}
	else {
		action = std::dynamic_pointer_cast<MoveAction>(ActionMaps::instance().getAction("DefaultMove"));
	}

	action->setDirection(direction);
	action->setStrafe(strafe);

	return action;

}

std::shared_ptr<RotateAction> ActionComponent::buildRotateAction(float angularVelocity) 
{

	std::shared_ptr<RotateAction> action;

	//Get the action if it exists, otherwise we will use the default actionClass for this action type
	std::optional<std::string> actionKey = _getActionKey(ACTION_ROTATE);

	if (actionKey) {
		action = std::dynamic_pointer_cast<RotateAction>(ActionMaps::instance().getAction(actionKey->data()));
	}
	else {
		action = std::dynamic_pointer_cast<RotateAction>(ActionMaps::instance().getAction("DefaultRotate"));
	}

	action->setAngularVelocity(angularVelocity);

	return action;
}

std::shared_ptr<Action> ActionComponent::buildUseAction() {


	std::shared_ptr<Action> action;

	//Get the action if it exists, otherwise we will use the default actionClass for this action type
	std::optional<std::string> actionKey = _getActionKey(ACTION_USE);

	if (actionKey) {
		action = ActionMaps::instance().getAction(actionKey->data());
	}
	else {
		action = ActionMaps::instance().getAction("NoAction");
	}

	return action;
}

std::shared_ptr<Action> ActionComponent::buildInteractAction() {

	std::shared_ptr<Action> action;

	//Get the action if it exists, otherwise we will use the default actionClass for this action type
	std::optional<std::string> actionKey = _getActionKey(ACTION_INTERACT);

	if (actionKey) {
		action = ActionMaps::instance().getAction(actionKey->data());
	}
	else {
		action = ActionMaps::instance().getAction("NoAction");
	}

	return action;
}

std::shared_ptr<Action> ActionComponent::buildOnHoverAction() {

	std::shared_ptr<Action> action;

	//Get the action if it exists, otherwise we will use the default actionClass for this action type
	std::optional<std::string> actionKey = _getActionKey(ACTION_ON_HOVER);

	if (actionKey) {
		action = ActionMaps::instance().getAction(actionKey->data());
	}
	else {
		action = ActionMaps::instance().getAction("DefaultHover");
	}

	return action;

}

std::shared_ptr<Action> ActionComponent::buildOnHoverOutAction() {

	std::shared_ptr<Action> action;

	//Get the action if it exists, otherwise we will use the default actionClass for this action type
	std::optional<std::string> actionKey = _getActionKey(ACTION_ON_HOVER_OUT);

	if (actionKey) {
		action = ActionMaps::instance().getAction(actionKey->data());
	}
	else {
		action = ActionMaps::instance().getAction("DefaultHoverOut");
	}

	return action;
}

std::shared_ptr<Action> ActionComponent::buildOnClickAction() {

	std::shared_ptr<Action> action;

	//Get the action if it exists, otherwise we will use the default actionClass for this action type
	std::optional<std::string> actionKey = _getActionKey(ACTION_ON_CLICK);

	if (actionKey) {
		action = ActionMaps::instance().getAction(actionKey->data());
	}
	else {
		action = ActionMaps::instance().getAction("NoAction");
	}

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

void ActionComponent::performOnHoverAction()
{
	buildOnHoverAction()->perform(m_parentGameObject);
}

void ActionComponent::performOnHoverOutAction()
{
	buildOnHoverOutAction()->perform(m_parentGameObject);
}

void ActionComponent::performOnClickAction()
{
	buildOnClickAction()->perform(m_parentGameObject);
}

std::optional<std::string> ActionComponent::_getActionKey(int actionType)
{

	if (m_actions.find(actionType) != m_actions.end()) {
		return m_actions.at(actionType);
	}
	else {
		return std::nullopt;
	}
	
}





