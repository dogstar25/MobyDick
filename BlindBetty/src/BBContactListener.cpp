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



void BBContactListener::_player_wall(GameObject* player, GameObject* wall, b2Vec2 contactPoint)
{

	//If we are in the middle of boosting and we are touching a wall at pointing towards the wall, then stop boosting
	//If we are not pointing at the wall then we are okay to slide/boost aganist it
	//const auto& playerControlComponent = player->getComponent<GinaPlayerControlComponent>(ComponentTypes::PLAYER_CONTROL_COMPONENT);

	//if (playerControlComponent->state() == PlayerState::boosting) {

	//	if (player->isPointingAt(wall->getCenterPosition())) {
	//		playerControlComponent->boostReset(false);
	//	}
	//	
	//}
	

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
	if ((contactTag1 == ContactTag::PLAYER_COLLISION && contactTag2 == ContactTag::WALL) ||
		(contactTag2 == ContactTag::PLAYER_COLLISION && contactTag1 == ContactTag::WALL)) {

		if (contactTag1 == ContactTag::PLAYER_COLLISION) {
			_player_wall(contact1, contact2, contactPoint);
		}
		else {
			_player_wall(contact2, contact1, contactPoint);
		}
	}


}






