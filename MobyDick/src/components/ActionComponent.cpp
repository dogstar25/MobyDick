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

	//Need to approach the "touching" logic from both the perspective of the player AND
	//the perspective of the interactive object because, because of box2d oddness, only one object or the other will
	//have the contact information

	if (parent()->hasTrait(TraitTag::interactive)) {
		int todd = 1;
	}

	//If this an interactiveObject and a playerObject is touching it, then display its interactive menu, if one exists
	if (parent()->hasTrait(TraitTag::interactive)) {

		GameObject* interactiveObject = parent();
		for (const auto& touchingObject : parent()->getTouchingObjects()) {

			if(touchingObject.second->hasTrait(TraitTag::player)) {

				GameObject* player = touchingObject.second;
				//Is the player pointing at this interactive object?
				if (player->isPointingAt(interactiveObject->getCenterPosition())) {

					const std::shared_ptr<ActionComponent> actionComponent = interactiveObject->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);

					if (actionComponent->interactiveMenuObject()) {
						SDL_FPoint position = determineInteractionMenuLocation(player, interactiveObject, actionComponent->interactiveMenuObject().get());

						actionComponent->interactiveMenuObject()->setPosition(position);
						actionComponent->interactiveMenuObject()->render();
					}

				}
			}
		}

	}




	//If this is a player object then see if it is touching a interactive object and display that interactives objects interactionMenu if it is
	//if (parent()->hasTrait(TraitTag::player)) {
	//	const std::shared_ptr<PhysicsComponent> physicsComponent = parent()->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);

	//	for (b2ContactEdge* edge = physicsComponent->physicsBody()->GetContactList(); edge; edge = edge->next)
	//	{
	//		b2Contact* contact = edge->contact;
	//		GameObject* contactGameObject = reinterpret_cast<GameObject*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);

	//		if (contactGameObject->hasTrait(TraitTag::interactive)) {

	//			if (contact->IsTouching() && parent()->isPointingAt(contactGameObject->getCenterPosition())) {

	//				const std::shared_ptr<ActionComponent> actionComponent = contactGameObject->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);

	//				if (actionComponent->interactiveMenuObject()) {
	//					SDL_FPoint position = determineInteractionMenuLocation(parent(), contactGameObject, actionComponent->interactiveMenuObject().get());
	//					
	//					actionComponent->interactiveMenuObject()->setPosition(position);
	//					actionComponent->interactiveMenuObject()->render();
	//					//we need to break because the player has an extra sensor for touching things which triggers this 
	//					//menu showing twice
	//					break;
	//				}
	//			}
	//		}
	//	}

	//}

	////If this is an interactive object then see if it is touching a player object and display that interactives objects interactionMenu if it is
	//if (parent()->hasTrait(TraitTag::interactive)) {
	//	const std::shared_ptr<PhysicsComponent> physicsComponent = parent()->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);

	//	for (b2ContactEdge* edge = physicsComponent->physicsBody()->GetContactList(); edge; edge = edge->next)
	//	{
	//		b2Contact* contact = edge->contact;
	//		GameObject* contactGameObject = reinterpret_cast<GameObject*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);

	//		if (contactGameObject->hasTrait(TraitTag::player)) {

	//			if (contact->IsTouching() && parent()->isPointingAt(contactGameObject->getCenterPosition())) {

	//				if (this->interactiveMenuObject()) {
	//					SDL_FPoint position = determineInteractionMenuLocation(parent(), contactGameObject, this->interactiveMenuObject().get());

	//					this->interactiveMenuObject()->setPosition(position);
	//					this->interactiveMenuObject()->render();
	//					//we need to break because the player has an extra sensor for touching things which triggers this 
	//					//menu showing twice
	//					break;
	//				}
	//			}
	//		}
	//	}

	//}

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

std::shared_ptr<Action> ActionComponent::getAction(int actionId)
{

	return m_actions[actionId];
}

void ActionComponent::postInit()
{

	//For any interactive object that has an interactiveMenuObject, we need to give that interactiveMenuObject a pointer
	//to the interactive object. Otherwise, it doesnt know what GameObject it controls
	for (auto& layer : parent()->parentScene()->gameObjects()) {

		for (auto& gameObject : layer) {

			if (gameObject->hasTrait(TraitTag::interactive)) {

				const auto& interactiveObjectActionComponent = gameObject->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);
				if (interactiveObjectActionComponent->interactiveMenuObject()) {

					auto interactiveMenuObject = interactiveObjectActionComponent->interactiveMenuObject();

					//Get the menu objects ImGui component
					const auto& menuObjectImGuiComponent = interactiveMenuObject->getComponent<IMGuiComponent>(ComponentTypes::IMGUI_COMPONENT);
					menuObjectImGuiComponent->setInteractionObjectActionComponent(gameObject);


				}

			}


		}
	}
}



