#include "ActionComponent.h"

#include <memory.h>

#include "../EnumMaps.h"
#include "../game.h"

#include "../actions/DefaultMoveAction.h"
#include "../actions/DefaultRotateAction.h"
#include "../actions/DefaultOnHoverAction.h"
#include "../actions/DefaultOnHoverOutAction.h"
#include "../IMGui/IMGuiItem.h"

extern std::unique_ptr<Game> game;

ActionComponent::ActionComponent(Json::Value componentJSON, Scene* parentScene)
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

	//Interactive object if exists
	if (componentJSON.isMember("interactiveMenuObject")) {
		auto interactiveMenuObjectId = componentJSON["interactiveMenuObject"].asString();
		m_interactiveMenuObject = std::make_shared<GameObject>(interactiveMenuObjectId, -5.f, -5.f, 0.f, parentScene);

	}

}

ActionComponent::~ActionComponent()
{

	m_actions.clear();

}

void ActionComponent::update()
{


}

void ActionComponent::render()
{

	//If this an interactiveObject and a playerObject is touching it, then display its interactive menu, if one exists
	if (parent()->hasTrait(TraitTag::interactive)) {

		for (const auto& touchingObject : parent()->getTouchingObjects()) {

			if(touchingObject.second->hasTrait(TraitTag::player)) {

				GameObject* interactingObject = touchingObject.second;
				//Is the interactingObject(player) pointing at this interactive object?
				if (interactingObject->isPointingAt(parent()->getCenterPosition())) {

					if (m_interactiveMenuObject) {
						SDL_FPoint position = determineInteractionMenuLocation(interactingObject, parent(), m_interactiveMenuObject.get());
						m_interactiveMenuObject->setPosition(position);
						m_interactiveMenuObject->render();
					}

				}
			}
		}

	}

}


SDL_FPoint ActionComponent::determineInteractionMenuLocation(GameObject* interactingObject, GameObject* contactGameObject, GameObject* menuObject)
{

	//is the ineteracting object(player) on the left or right side of the interaction object
	float orientationAngle = atan2(interactingObject->getCenterPosition().y - contactGameObject->getCenterPosition().y, 
		interactingObject->getCenterPosition().x - contactGameObject->getCenterPosition().x);
	float orientationAngleDegrees = util::radiansToDegrees(orientationAngle);

	bool rightside = false;
	if (abs(orientationAngleDegrees) < 90) {

		rightside = true;
	}

	float xPos{};
	float yPos{};
	if (rightside == false) {

		xPos = contactGameObject->getCenterPosition().x + (contactGameObject->getSize().x/2) + (menuObject->getSize().x / 2);
		yPos = contactGameObject->getCenterPosition().y + (contactGameObject->getSize().y / 2);

	}
	else {
		xPos = contactGameObject->getCenterPosition().x - (contactGameObject->getSize().x/2) - (menuObject->getSize().x / 2);
		yPos = contactGameObject->getCenterPosition().y + (contactGameObject->getSize().y / 2);

	}


	return SDL_FPoint{xPos, yPos};


}

void ActionComponent::setParent(GameObject* gameObject)
{
	//Call base component setParent
	Component::setParent(gameObject);

	//Parent for this interactionMenuObject if it exists
	if (m_interactiveMenuObject) {
		m_interactiveMenuObject->setParent(gameObject);

	}

}

std::shared_ptr<Action> ActionComponent::getAction(int actionId)
{

	return m_actions[actionId];
}




