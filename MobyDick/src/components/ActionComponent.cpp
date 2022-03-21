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


std::shared_ptr<Action> ActionComponent::getAction(int actionId)
{

	return m_actions[actionId];
}



