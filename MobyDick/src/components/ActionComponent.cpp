#include "ActionComponent.h"

#include <cassert>

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

	m_actions.clear();

}

void ActionComponent::update()
{


}

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

	assert(action != nullptr && "Move Action is null!");
	action->perform(m_parentGameObject);
}

void ActionComponent::performMoveAction(b2Vec2 trajectory)
{
	std::shared_ptr<MoveAction> action;

	if (m_actions[ACTION_MOVE]) {

		action = std::dynamic_pointer_cast<MoveAction>(m_actions[ACTION_MOVE]);
		action->setTrajectory(trajectory);
	}
	else {
		action = std::dynamic_pointer_cast<MoveAction>(ActionMaps::instance().getAction("DefaultMove"));
	}

	assert(action != nullptr && "Move Action is null!");
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

	assert(action != nullptr && "Rotate Action is null!");
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

	assert(action != nullptr && "Use Action is null!");
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

	assert(action != nullptr && "Usage Action is null!");
	action->perform(m_parentGameObject);
}

void ActionComponent::performInteractAction(std::tuple<std::string, int, float> params)
{
	std::shared_ptr<InteractAction> action;

	if (m_actions[ACTION_INTERACT]) {

		action = std::dynamic_pointer_cast<InteractAction>(m_actions[ACTION_INTERACT]);
		action->setParams(params);
	}
	else {
		action = std::dynamic_pointer_cast<InteractAction>(ActionMaps::instance().getAction("Interact"));
	}

	assert(action != nullptr && "Action is null!");
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

	assert(action != nullptr && "Action is null!");
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

	assert(action != nullptr && "Action is null!");
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

	assert(action != nullptr && "Action is null!");
	action->perform(m_parentGameObject);
}

void ActionComponent::performTriggerAction()
{
	std::shared_ptr<Action> action;

	if (m_actions[ACTION_ON_TRIGGER]) {

		action = std::dynamic_pointer_cast<Action>(m_actions[ACTION_ON_TRIGGER]);
	}
	else {
		action = std::dynamic_pointer_cast<Action>(ActionMaps::instance().getAction("NoAction"));
	}

	assert(action != nullptr && "Action is null!");
	action->perform();
}



