#include "InteractAction.h"

#include "../components/PhysicsComponent.h"
#include <memory>

void InteractAction::perform(GameObject* gameObject, Json::Value actionParms)
{
	const std::shared_ptr<PhysicsComponent> physicsComponent = gameObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);

	for (b2ContactEdge* edge = physicsComponent->physicsBody()->GetContactList(); edge; edge = edge->next)
	{
		b2Contact* contact = edge->contact;

		if (contact->IsTouching()) {
			GameObject* contactGameObject = reinterpret_cast<GameObject*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);

			if (contactGameObject->hasTrait(TraitTag::interactive)) {


				//const auto& actionComponent = contactGameObject->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);
				//Json::Value runtimeParms(Json::objectValue);
				//runtimeParms["???"] = "what";
				//auto interactionAction = actionComponent->getAction(ACTION_INTERACTION);
				//interactionAction->perform(contactGameObject);

			}
		}
	}

}