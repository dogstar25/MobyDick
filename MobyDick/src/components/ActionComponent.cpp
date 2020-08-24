#include "ActionComponent.h"


#include "../actions/ActionMaps.h"
#include "../EnumMaps.h"


ActionComponent::ActionComponent(Json::Value definitionJSON)
{
	Json::Value componentJSON = definitionJSON["actionComponent"];

	m_actions.resize(32);
	for (Json::Value itrAction: componentJSON["actions"])
	{
		//Get the name of the class to be used as the action as a string
		std::string actionCode = itrAction["actionClass"].asString();

		//Get the Enum that represents the Game Objects action as an int
		int actionId = EnumMap::instance().toEnum(itrAction["actionId"].asString());


		m_actions[actionId] = ActionMaps::instance().getAction(actionCode);


	}

}

ActionComponent::~ActionComponent()
{

}

void ActionComponent::update()
{


}

//std::shared_ptr<MoveAction> ActionComponent::buildMoveAction(int direction, int strafe) 
//{
//
//	std::shared_ptr<MoveAction> action;
//
//	//Get the action if it exists, otherwise we will use the default actionClass for this action type
//	std::optional<std::string> actionKey = _getActionKey(ACTION_MOVE);
//
//	if (actionKey) {
//		action = std::dynamic_pointer_cast<MoveAction>(ActionMaps::instance().getAction(actionKey->data()));
//	}
//	else {
//		action = std::dynamic_pointer_cast<MoveAction>(ActionMaps::instance().getAction("DefaultMove"));
//	}
//
//	action->setDirection(direction);
//	action->setStrafe(strafe);
//
//	return action;
//
//}
//
//std::shared_ptr<RotateAction> ActionComponent::buildRotateAction(float angularVelocity) 
//{
//
//	std::shared_ptr<RotateAction> action;
//
//	//Get the action if it exists, otherwise we will use the default actionClass for this action type
//	std::optional<std::string> actionKey = _getActionKey(ACTION_ROTATE);
//
//	if (actionKey) {
//		action = std::dynamic_pointer_cast<RotateAction>(ActionMaps::instance().getAction(actionKey->data()));
//	}
//	else {
//		action = std::dynamic_pointer_cast<RotateAction>(ActionMaps::instance().getAction("DefaultRotate"));
//	}
//
//	action->setAngularVelocity(angularVelocity);
//
//	return action;
//}
//
//std::shared_ptr<Action> ActionComponent::buildUseAction() {
//
//
//	std::shared_ptr<Action> action;
//
//	//Get the action if it exists, otherwise we will use the default actionClass for this action type
//	std::optional<std::string> actionKey = _getActionKey(ACTION_USE);
//
//	if (actionKey) {
//		action = ActionMaps::instance().getAction(actionKey->data());
//	}
//	else {
//		action = ActionMaps::instance().getAction("NoAction");
//	}
//
//	return action;
//}
//
//std::shared_ptr<Action> ActionComponent::buildInteractAction() {
//
//	std::shared_ptr<Action> action;
//
//	//Get the action if it exists, otherwise we will use the default actionClass for this action type
//	std::optional<std::string> actionKey = _getActionKey(ACTION_INTERACT);
//
//	if (actionKey) {
//		action = ActionMaps::instance().getAction(actionKey->data());
//	}
//	else {
//		action = ActionMaps::instance().getAction("NoAction");
//	}
//
//	return action;
//}
//
//std::shared_ptr<Action> ActionComponent::buildOnHoverAction() {
//
//	std::shared_ptr<Action> action;
//
//	//Get the action if it exists, otherwise we will use the default actionClass for this action type
//	std::optional<std::string> actionKey = _getActionKey(ACTION_ON_HOVER);
//
//	if (actionKey) {
//		action = ActionMaps::instance().getAction(actionKey->data());
//	}
//	else {
//		action = ActionMaps::instance().getAction("DefaultHover");
//	}
//
//	return action;
//
//}
//
//std::shared_ptr<Action> ActionComponent::buildOnHoverOutAction() {
//
//	std::shared_ptr<Action> action;
//
//	//Get the action if it exists, otherwise we will use the default actionClass for this action type
//	std::optional<std::string> actionKey = _getActionKey(ACTION_ON_HOVER_OUT);
//
//	if (actionKey) {
//		action = ActionMaps::instance().getAction(actionKey->data());
//	}
//	else {
//		action = ActionMaps::instance().getAction("DefaultHoverOut");
//	}
//
//	return action;
//}
//
//std::shared_ptr<Action> ActionComponent::buildOnClickAction() {
//
//	std::shared_ptr<Action> action;
//
//	//Get the action if it exists, otherwise we will use the default actionClass for this action type
//	std::optional<std::string> actionKey = _getActionKey(ACTION_ON_CLICK);
//
//	if (actionKey) {
//		action = ActionMaps::instance().getAction(actionKey->data());
//	}
//	else {
//		action = ActionMaps::instance().getAction("NoAction");
//	}
//
//	return action;
//}

void ActionComponent::performMoveAction(int direction, int strafe)
{
	std::shared_ptr<MoveAction> action;

	if (m_actions[ACTION_MOVE]) {

		action = std::dynamic_pointer_cast<MoveAction>(m_actions[ACTION_MOVE]);
		action->setDirection(direction);
		action->setStrafe(strafe);
	}
	else {
		action = std::dynamic_pointer_cast<MoveAction>(ActionMaps::instance().getAction("DefaultMove"));
	}
	action->perform(m_parentGameObject);
}

void ActionComponent::performRotateAction(float angularVelocity)
{
	std::shared_ptr<RotateAction> action;

	if (m_actions[ACTION_ROTATE]) {

		action = std::dynamic_pointer_cast<RotateAction>(m_actions[ACTION_ROTATE]);
		action->setAngularVelocity(angularVelocity);
	}
	else{
		action = std::dynamic_pointer_cast<RotateAction>(ActionMaps::instance().getAction("DefaultRotate"));
	}
	action->perform(m_parentGameObject);
}

void ActionComponent::performUseAction()
{
	std::shared_ptr<Action> action;

	if (m_actions[ACTION_USE]) {
		action = m_actions[ACTION_USE];
	}
	else {
		action = ActionMaps::instance().getAction("NoAction");
	}

	action->perform(m_parentGameObject);
}

void ActionComponent::performUsageAction()
{
	std::shared_ptr<Action> action;

	if (m_actions[ACTION_USAGE]) {
		action = m_actions[ACTION_USAGE];
	}
	else {
		action = ActionMaps::instance().getAction("NoAction");
	}

	action->perform(m_parentGameObject);
}

void ActionComponent::performInteractAction()
{
	std::shared_ptr<Action> action;

	if (m_actions[ACTION_INTERACT]) {

		action = m_actions[ACTION_INTERACT];
	}
	else {
		action = ActionMaps::instance().getAction("NoAction");
	}

	action->perform(m_parentGameObject);
}

void ActionComponent::performOnHoverAction()
{
	std::shared_ptr<Action> action;

	if (m_actions[ACTION_ON_HOVER]) {

		action = m_actions[ACTION_ON_HOVER];
	}
	else {
		action = ActionMaps::instance().getAction("DefaultHover");
	}

	action->perform(m_parentGameObject);
}

void ActionComponent::performOnHoverOutAction()
{
	std::shared_ptr<Action> action;

	if (m_actions[ACTION_ON_HOVER_OUT]) {
		action = m_actions[ACTION_ON_HOVER_OUT];
	}
	else {
		action = ActionMaps::instance().getAction("DefaultHoverOut");
	}
	action->perform(m_parentGameObject);
}

void ActionComponent::performOnClickAction()
{
	std::shared_ptr<Action> action;

	if (m_actions[ACTION_ON_CLICK]) {

		action = m_actions[ACTION_ON_CLICK];
	}
	else {
		action = ActionMaps::instance().getAction("NoAction");
	}

	action->perform(m_parentGameObject);
}





