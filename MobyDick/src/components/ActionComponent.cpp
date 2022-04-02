#include "ActionComponent.h"

#include <memory.h>

#include "../EnumMaps.h"
#include "../game.h"

#include "../actions/DefaultMoveAction.h"
#include "../actions/DefaultRotateAction.h"
#include "../actions/DefaultOnHoverAction.h"
#include "../actions/DefaultOnHoverOutAction.h"

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
	if (componentJSON.isMember("interactiveHintObject")) {
		auto interactiveHintObjectId = componentJSON["interactiveHintObject"].asString();
		m_interactiveHintObject = std::make_shared<GameObject>(interactiveHintObjectId, -5.f, -5.f, 0.f, parentScene);

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

	//If this is a player object then see if it is touching a interactive object and display that interactives objects interactionMenu if it is
	if (parent()->hasTrait(TraitTag::player)) {
		const std::shared_ptr<PhysicsComponent> physicsComponent = parent()->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);

		for (b2ContactEdge* edge = physicsComponent->physicsBody()->GetContactList(); edge; edge = edge->next)
		{
			b2Contact* contact = edge->contact;
			GameObject* contactGameObject = reinterpret_cast<GameObject*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);

			if (contactGameObject->hasTrait(TraitTag::interactive)) {

				if (contact->IsTouching() && parent()->isPointingAt(contactGameObject->getCenterPosition())) {

					const std::shared_ptr<ActionComponent> actionComponent = contactGameObject->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);

					if (actionComponent->interactiveHintObject()) {
						SDL_FPoint position = determineInteractionMenuLocation(parent(), contactGameObject, actionComponent->interactiveHintObject().get());
						actionComponent->interactiveHintObject()->setPosition(position);
						actionComponent->interactiveHintObject()->render();
					}
				}
			}
		}

	}

}


SDL_FPoint ActionComponent::determineInteractionMenuLocation(GameObject* interactingObject, GameObject* contactGameObject, GameObject* hintMenuObject)
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

		xPos = contactGameObject->getCenterPosition().x + contactGameObject->getSize().x + (hintMenuObject->getSize().x / 2);
		yPos = contactGameObject->getCenterPosition().y + (contactGameObject->getSize().y / 2);

	}
	else {
		xPos = contactGameObject->getCenterPosition().x - contactGameObject->getSize().x - (hintMenuObject->getSize().x / 2);
		yPos = contactGameObject->getCenterPosition().y + (contactGameObject->getSize().y / 2);

	}


	return SDL_FPoint{xPos, yPos};


}

std::shared_ptr<Action> ActionComponent::getAction(int actionId)
{

	return m_actions[actionId];
}



