#include "ActionComponent.h"


#include "../EnumMaps.h"
#include "../game.h"
#include "../actions/DefaultMoveAction.h"
#include "../actions/DefaultRotateAction.h"
#include "../actions/DefaultOnHoverAction.h"
#include "../actions/DefaultOnHoverOutAction.h"

extern std::unique_ptr<Game> game;

ActionComponent::ActionComponent(Json::Value componentJSON)
{
	m_actions.resize(32);
	for (Json::Value itrAction: componentJSON["actions"])
	{
		//Get the name of the class to be used as the action as a string
		std::string actionClass = itrAction["actionClass"].asString();

		//Get the Enum that represents the Game Objects action as an int
		int actionId = EnumMap::instance().toEnum(itrAction["actionId"].asString());

		m_actions[actionId] = game->actionFactory()->create(actionClass);

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
		action = std::make_shared<DefaultMoveAction>();
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
		action = std::make_shared<DefaultMoveAction>();
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
		action = std::make_shared<DefaultRotateAction>();
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
		action = std::make_shared<NoAction>();
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
		action = std::make_shared<NoAction>();
	}

	assert(action != nullptr && "Usage Action is null!");
	action->perform(m_parentGameObject);
}

void ActionComponent::performInteractAction(std::tuple<std::string, int, float> params)
{
	std::shared_ptr<Action> action;

	if (m_actions[ACTION_INTERACT]) {

		action = std::dynamic_pointer_cast<InteractAction>(m_actions[ACTION_INTERACT]);
	}
	else {
		action = std::make_shared<NoAction>();
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
		action = std::make_shared<DefaultOnHoverAction>();
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
		action = std::make_shared<DefaultOnHoverOutAction>();
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
		action = std::make_shared<NoAction>();
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
		action = std::make_shared<NoAction>();
	}

	assert(action != nullptr && "Action is null!");
	action->perform();
}

std::shared_ptr<Action> ActionComponent::getAction(int actionId)
{

	return m_actions[actionId];
}



