#include "MRContactListener.h"
#include "SceneManager.h"

#include <iostream>

#include "MRContextManager.h"
#include "game.h"
#include "GameConstants.h"
#include "SoundManager.h"
#include "particleEffects/GameParticleEffects.h"
#include "components/InventoryComponent.h"
#include "components/PistolWeaponComponent.h"

extern std::unique_ptr<Game> game;

void MRContactListener::_player_wall(GameObject* contact1, GameObject* contact2, b2Vec2 contactPoint)
{
	GameObject* player;
	GameObject* wall;

	if (contact1->collisionTag() == CollisionTag::PLAYER) {
		player = contact1;
		wall = contact2;
	}
	else {
		player = contact2;
		wall = contact1;
	}

	//do What?


}

void MRContactListener::_player_interactive(GameObject* contact1, GameObject* contact2, b2Vec2 contactPoint)
{
	GameObject* player;
	GameObject* interactiveObject;

	if (contact1->collisionTag() == CollisionTag::PLAYER) {
		player = contact1;
		interactiveObject = contact2;
	}
	else {
		player = contact2;
		interactiveObject = contact1;
	}

	//Get the interactive objects menu
	auto interactiveActionComponent = interactiveObject->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);

	//Put the usuable indicator somewhere in the area
	auto interactionMenuObject = interactiveActionComponent->interactiveMenuObject();
	interactionMenuObject->setPosition(interactiveObject->getCenterPosition().x, interactiveObject->getCenterPosition().y);
	interactionMenuObject->enable();


}

void MRContactListener::_player_interactive_end(GameObject* contact1, GameObject* contact2, b2Vec2 contactPoint)
{

	GameObject* player;
	GameObject* interactiveObject;

	if (contact1->collisionTag() == CollisionTag::PLAYER) {
		player = contact1;
		interactiveObject = contact2;
	}
	else {
		player = contact2;
		interactiveObject = contact1;
	}

	//Get the interactive objects menu
	auto interactiveActionComponent = interactiveObject->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);

	//Put the usuable indicator somewhere in the area
	auto interactionMenuObject = interactiveActionComponent->interactiveMenuObject();
	interactionMenuObject->disable(false);









}

void MRContactListener::_bullet_wall(GameObject* contact1, GameObject* contact2, b2Vec2 contactPoint)
{
	GameObject* bullet;
	GameObject* wall;

	if (contact1->collisionTag() == CollisionTag::PLAYER_BULLET || 
		contact1->collisionTag() == CollisionTag::ENEMY_BULLET) {

		bullet = contact1;
		wall = contact2;
	}
	else {
		bullet = contact2;
		wall = contact1;
	}

	//Build a One-Time particle emitter object
	auto particleEmitterObject = SceneManager::instance().addGameObject("PARTICLE_X_EMITTER", LAYER_MAIN, -1, -1);
	const auto& particleComponent = particleEmitterObject->getComponent<ParticleComponent>(ComponentTypes::PARTICLE_X_COMPONENT);
	particleComponent->addParticleEffect(ParticleEffects::ricochet);

	//Convert from box2d to gameWorld coordinates
	contactPoint.x *= GameConfig::instance().scaleFactor();
	contactPoint.y *= GameConfig::instance().scaleFactor();
	particleEmitterObject->setPosition(contactPoint.x, contactPoint.y);

	//Set flag for removal for the Bullet
	bullet->setRemoveFromWorld(true);

}

void MRContactListener::_playerBullet_droneShield(GameObject* contact1, GameObject* contact2, b2Vec2 contactPoint)
{
	GameObject* bullet;
	GameObject* shield;

	if (contact1->collisionTag() == CollisionTag::PLAYER_BULLET) {
		bullet = contact1;
		shield = contact2;
	}
	else {
		bullet = contact2;
		shield = contact1;
	}


	auto particleEmitterObject = SceneManager::instance().addGameObject("PARTICLE_X_EMITTER", LAYER_MAIN, -1, -1);
	auto particleComponent = particleEmitterObject->getComponent<ParticleXComponent>(ComponentTypes::PARTICLE_X_COMPONENT);
	//auto particleEmitterObject = SceneManager::instance().addGameObject("PARTICLE_EMITTER", LAYER_MAIN, -1, -1);
	//auto particleComponent = particleEmitterObject->getComponent<ParticleComponent>(ComponentTypes::PARTICLE_COMPONENT);

	particleComponent->setType(ParticleEmitterType::ONETIME);
	//particleComponent->setType(ParticleEmitterType::CONTINUOUS);

	//Convert from box2d to gameWorld coordinates
	contactPoint.x *= GameConfig::instance().scaleFactor();
	contactPoint.y *= GameConfig::instance().scaleFactor();
	particleEmitterObject->setPosition(contactPoint.x, contactPoint.y);

	//Set flag for removal for the Bullet
	bullet->setRemoveFromWorld(true);

	//Test if the bullet is strong enought to destroy the shield piece
	auto bulletVitality = bullet->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);
	auto shieldVitality = shield->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);
	auto shieldHolds = shieldVitality->testResistance(bulletVitality->attackPower());
	if (shieldHolds == false) {

		particleComponent->addParticleEffect(ParticleEffects::impactSmoke);
		particleComponent->addParticleEffect(ParticleEffects::scrap);
		SoundManager::instance().playSound("SFX_IMPACT_1");

	}
	else {

		particleComponent->addParticleEffect(ParticleEffects::deflect);
		SoundManager::instance().playSound("SFX_IMPACT_3");
	}

}

void MRContactListener::_player_shieldScrap(GameObject* contact1, GameObject* contact2, b2Vec2 contactPoint)
{

	GameObject* player;
	GameObject* scrap;

	if (contact1->collisionTag() == CollisionTag::PLAYER) {
		player = contact1;
		scrap = contact2;
	}
	else {
		scrap = contact1;
		player = contact2;
	}

	//Acknowledge the scrap collection
	auto inventoryComponent = player->getComponent<InventoryComponent>(ComponentTypes::INVENTORY_COMPONENT);
	auto scrapCount = inventoryComponent->addCollectible(CollectibleTypes::DRONE_SCRAP, 1);

	//Update the status Manager
	//static_cast<MRStatusManager*>(game->statusMananger())->hudValueMap()["HUD_SCRAP_VALUE"].adjust(1);
	game->contextMananger()->adjustValue("SCRAP_COUNT", 1);

	//Check to see if this upgrades the players weapon
	auto pistol = inventoryComponent->getItem(TraitTag::weapon);
	if (pistol.has_value()) {
		auto pistolWeaponComponent = pistol.value()->getComponent<PistolWeaponComponent>(ComponentTypes::WEAPON_COMPONENT);
		pistolWeaponComponent->checkLevelUp(scrapCount);
	}
	
	//flag the scrap item to be removed from the game and play a sound effect
	scrap->setRemoveFromWorld(true);
	SoundManager::instance().playSound("SFX_PICKUP_2");

}

void MRContactListener::_bullet_player(GameObject* contact1, GameObject* contact2, b2Vec2 contactPoint)
{

	GameObject* player;
	GameObject* bullet;

	if (contact1->collisionTag() == CollisionTag::PLAYER) {
		player = contact1;
		bullet = contact2;
	}
	else {
		bullet = contact1;
		player = contact2;
	}

	//Update the status Manager
	game->contextMananger()->adjustValue("LIVES_COUNT", -1);

	//flag the scrap item to be removed from the game and play a sound effect
	bullet->setRemoveFromWorld(true);
	SoundManager::instance().playSound("SFX_PICKUP_2");

}
void MRContactListener::BeginContact(b2Contact* contact) {


	//Get the 2 object pointers
	GameObject* gameObjectA = reinterpret_cast<GameObject*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
	GameObject* gameObjectB = reinterpret_cast<GameObject*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

	float x = 0;
	float y = 0;
	b2WorldManifold worldManifold;

	contact->GetWorldManifold(&worldManifold);
	b2Vec2 contactPoint = worldManifold.points[0];

	if (gameObjectA != nullptr && gameObjectB != nullptr)
	{
		handleBeginContact(gameObjectA, gameObjectB, contactPoint);
	}

}


void MRContactListener::EndContact(b2Contact* contact)
{

	//Get the 2 object pointers
	GameObject* gameObjectA = reinterpret_cast<GameObject*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
	GameObject* gameObjectB = reinterpret_cast<GameObject*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

	float x = 0;
	float y = 0;
	b2WorldManifold worldManifold;

	contact->GetWorldManifold(&worldManifold);
	b2Vec2 contactPoint = worldManifold.points[0];

	if (gameObjectA != nullptr && gameObjectB != nullptr)
	{
		handleEndContact(gameObjectA, gameObjectB, contactPoint);
	}


}



void MRContactListener::handleBeginContact(GameObject* contact1, GameObject* contact2, b2Vec2 contactPoint)
{

	ContactListener::handleBeginContact(contact1, contact2, contactPoint);

	auto category1 = contact1->collisionTag();
	auto category2 = contact2->collisionTag();
	auto traits1 = contact1->traits();
	auto traits2 = contact2->traits();

	////////////////////////////////
	// Player Usuable Object Contact 
	////////////////////////////////
	if ((category1 == CollisionTag::PLAYER && contact2->hasTrait(TraitTag::interactive)) ||
		(category2 == CollisionTag::PLAYER && contact1->hasTrait(TraitTag::interactive))) {

		//_player_interactive(contact1, contact2, contactPoint);
	}

	/////////////////////////
	// Player Wall Contact
	////////////////////////
	if ((category1 == CollisionTag::PLAYER && category2 == CollisionTag::WALL) ||
		(category2 == CollisionTag::PLAYER && category1 == CollisionTag::WALL)) {

		_player_wall(contact1, contact2, contactPoint);

	}

	////////////////////////////////////
	// Player Bullet -  Wall Contact
	//////////////////////////////////
	if ((category1 == CollisionTag::PLAYER_BULLET && category2 == CollisionTag::WALL) ||
		(category2 == CollisionTag::PLAYER_BULLET && category1 == CollisionTag::WALL)) {

		_bullet_wall(contact1, contact2, contactPoint);

	}

	////////////////////////////////////
	// Enemy Bullet -  Wall Contact
	//////////////////////////////////
	if ((category1 == CollisionTag::ENEMY_BULLET && category2 == CollisionTag::WALL) ||
		(category2 == CollisionTag::ENEMY_BULLET && category1 == CollisionTag::WALL)) {

		_bullet_wall(contact1, contact2, contactPoint);

	}

	///////////////////////////////////
	// Player Bullet -  Drone Shield
	///////////////////////////////////
	if ((category1 == CollisionTag::PLAYER_BULLET && category2 == CollisionTag::DRONE_SHIELD) ||
		(category2 == CollisionTag::PLAYER_BULLET && category1 == CollisionTag::DRONE_SHIELD)) {

		_playerBullet_droneShield(contact1, contact2, contactPoint);

	}

	///////////////////////////////////
	// Player - Drone Scrap
	///////////////////////////////////
	if ((category1 == CollisionTag::PLAYER && category2 == CollisionTag::SHIELD_SCRAP) ||
		(category2 == CollisionTag::PLAYER && category1 == CollisionTag::SHIELD_SCRAP)) {

		_player_shieldScrap(contact1, contact2, contactPoint);

	}

	///////////////////////////////////
	// Enemy Bullet - Player
	///////////////////////////////////
	if ((category1 == CollisionTag::PLAYER && category2 == CollisionTag::ENEMY_BULLET) ||
		(category2 == CollisionTag::PLAYER && category1 == CollisionTag::ENEMY_BULLET)) {

		_bullet_player(contact1, contact2, contactPoint);

	}

}




void MRContactListener::handleEndContact(GameObject* contact1, GameObject* contact2, b2Vec2 contactPoint)
{

	ContactListener::handleEndContact(contact1, contact2, contactPoint);

	auto category1 = contact1->collisionTag();
	auto category2 = contact2->collisionTag();
	auto traits1 = contact1->traits();
	auto traits2 = contact2->traits();

	////////////////////////////////
	// Player Usuable Object Contact 
	////////////////////////////////
	if ((category1 == CollisionTag::PLAYER && contact2->hasTrait(TraitTag::interactive)) ||
		(category2 == CollisionTag::PLAYER && contact1->hasTrait(TraitTag::interactive))) {

		//_player_interactive_end(contact1, contact2, contactPoint);
	}

}






