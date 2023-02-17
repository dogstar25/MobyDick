#include "BBContactListener.h"
#include "SceneManager.h"

#include <iostream>

#include "BBContextManager.h"
#include "game.h"
#include "GameConstants.h"
#include "SoundManager.h"
#include "particleEffects/GameParticleEffects.h"
#include "components/InventoryComponent.h"
#include "components/BobbyPlayerControlComponent.h"
#include "components/AttachmentsComponent.h"

extern std::unique_ptr<Game> game;



void BBContactListener::_actor_warpEntry(GameObject* interactingObject, GameObject* warpEntry, b2Vec2 contactPoint)
{

	const auto& physicsComponent = interactingObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);

	//build name for the warp exit object 
	std::string warpExitName = warpEntry->name() + "_EXIT";

	//Find this warp interactions exit
	const auto& exit = interactingObject->parentScene()->getFirstGameObjectByName(warpExitName);
	if (exit.has_value()) {

		//Get location and use it as warp destination
		b2Vec2 exitLocation = { exit.value()->getCenterPosition().x, exit.value()->getCenterPosition().y };
		util::toBox2dPoint(exitLocation);
		physicsComponent->setChangePositionPosition(exitLocation);

	}

}


void BBContactListener::BeginContact(b2Contact* contact) {

	b2WorldManifold worldManifold;

	contact->GetWorldManifold(&worldManifold);
	b2Vec2 contactPoint = worldManifold.points[0];

	handleContact(contact, contactPoint);

}


void BBContactListener::EndContact(b2Contact* contact)
{


}



void BBContactListener::handleContact(b2Contact* contact, b2Vec2 contactPoint)
{


	//Get fixtures
	b2Fixture* fixture1 = contact->GetFixtureA();
	b2Fixture* fixture2 = contact->GetFixtureB();


	//Get the GameObjects attched to these fixtures
	GameObject* contact1 = reinterpret_cast<GameObject*>(fixture1->GetBody()->GetUserData().pointer);
	GameObject* contact2 = reinterpret_cast<GameObject*>(fixture2->GetBody()->GetUserData().pointer);

	ContactDefinition* contactDefinitionA = reinterpret_cast<ContactDefinition*>(fixture1->GetUserData().pointer);
	ContactDefinition* contactDefinitionB = reinterpret_cast<ContactDefinition*>(fixture2->GetUserData().pointer);

	int contactTag1 = contactDefinitionA->contactTag;
	int contactTag2 = contactDefinitionB->contactTag;


	////////////////////////////////////
	// Player -  Wall
	//////////////////////////////////
	if ((contactTag1 == ContactTag::PLAYER_COLLISION && contactTag2 == ContactTag::WARP_ENTRY) ||
		(contactTag2 == ContactTag::PLAYER_COLLISION && contactTag1 == ContactTag::WARP_ENTRY)) {

		if (contactTag1 == ContactTag::PLAYER_COLLISION) {
			_actor_warpEntry(contact1, contact2, contactPoint);
		}
		else {
			_actor_warpEntry(contact2, contact1, contactPoint);
		}
	}


}






