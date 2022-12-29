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
#include "components/GinaPlayerControlComponent.h"
#include "components/GinaVitalityComponent.h"
#include "components/DroneBrainComponent.h"
#include "components/AttachmentsComponent.h"

extern std::unique_ptr<Game> game;


void MRContactListener::_playerBullet_droneBrain(GameObject* playerBullet, GameObject* droneBrain, b2Vec2 contactPoint)
{

	//Convert from box2d to gameWorld coordinates
	contactPoint.x *= GameConfig::instance().scaleFactor();
	contactPoint.y *= GameConfig::instance().scaleFactor();

	//Create a physics emitter for the dynamic chunks in the explosion
	auto particleXEmitter = SceneManager::instance().addGameObject("PARTICLE_X_EMITTER", GameLayer::MAIN, -1, -1);
	auto particleXComponent = particleXEmitter->getComponent<ParticleXComponent>(ComponentTypes::PARTICLE_X_COMPONENT);
	//set the explosion origin as the ceneter of the brain thats exploding - not the contact point
	particleXEmitter->setPosition(droneBrain->getCenterPosition());
	particleXComponent->setType(ParticleEmitterType::ONETIME);

	//Create a non-physics emitter for the extra smoke and stufff to exagerate the explosion
	//auto particleEmitter = SceneManager::instance().addGameObject("PARTICLE_EMITTER", GameLayer::MAIN, -1, -1);
	//auto particleComponent = particleEmitter->getComponent<ParticleComponent>(ComponentTypes::PARTICLE_COMPONENT);
	//particleEmitter->setPosition(contactPoint.x, contactPoint.y);
	//particleComponent->setType(ParticleEmitterType::ONETIME);

	//Set flag for removal for the Bullet
	playerBullet->setRemoveFromWorld(true);
	//droneBrain->setRemoveFromWorld(true);

	//Test if the bullet is strong enought to destroy the object
	auto bulletVitality = playerBullet->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);
	auto brainVitality = droneBrain->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);
	auto brainHolds = brainVitality->testResistance(bulletVitality->attackPower());
	if (brainHolds == false) {

		particleXComponent->addParticleEffect(ParticleEffects::impactSmoke);
		particleXComponent->addParticleEffect(ParticleEffects::turretScrap);
		particleXComponent->addParticleEffect(ParticleEffects::explosionSmoke);
		particleXComponent->addParticleEffect(ParticleEffects::pulseExplosion);

		//Get the Drone object
		auto droneObject = droneBrain->parent();

		//Stop all of the drone sounds
		const auto& droneSoundComponent = droneObject.value()->getComponent <SoundComponent>(ComponentTypes::SOUND_COMPONENT);
		droneSoundComponent->stopSounds();

		//Play destroyyed sound
		const auto& soundComponent = droneObject.value()->getComponent<SoundComponent>(ComponentTypes::SOUND_COMPONENT);
		soundComponent->playSound("DESTROYED_SOUND");

		//Hide the drone base and set it to dbe removed in 5 seconds
		//droneObject.value()->disableRender();
		droneObject.value()->disableCollision();
		const auto& droneVitalityComponent = droneObject.value()->getComponent <VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);
		droneVitalityComponent->setLifetimeTimer(4);
		droneVitalityComponent->setIsLifetimeAlphaFade(true);

		//Set the drones composite component to detach all of its pieces from its body
		const auto& droneCompositeComponent = droneObject.value()->getComponent<CompositeComponent>(ComponentTypes::COMPOSITE_COMPONENT);
		droneCompositeComponent->setDetachAllPieces();

		//remove the drones brain
		const auto& droneBrainComponent = droneObject.value()->getComponent<DroneBrainComponent>(ComponentTypes::BRAIN_COMPONENT);
		droneBrainComponent->disable();

		//remove the drones attachmenmts - physical brain and eye gun
		const auto& droneAttachmentComponent = droneObject.value()->getComponent<AttachmentsComponent>(ComponentTypes::ATTACHMENTS_COMPONENT);
		//droneAttachmentComponent->removeAllAttachments();
		droneAttachmentComponent->removeAttachment("DRONE_BRAIN");

		//set all of the composite pieces to:
		//		have a limited lifetime to fade out
		//		detach them from their base so they can bump around
		for (auto piece : droneCompositeComponent->pieces()) {

			//Make all pieces collide
			piece.pieceObject->setCollisionTag(ContactTag::DRONE_SCRAP);

			//Make fade away
			const auto& pieceVitalityComponent = piece.pieceObject->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);
			pieceVitalityComponent->setIsLifetimeAlphaFade(true);
			pieceVitalityComponent->setLifetimeTimer(4);

		}

		//Update the status tracker for enemy count
		if (game->contextMananger()->getStatusItem(StatusItemId::ENEMY_DRONE_COUNT).id() != StatusItemId::DEFAULT_EMPTY) {
			game->contextMananger()->adjustStatusItemValue(StatusItemId::ENEMY_DRONE_COUNT, -1);
		}


	}
	else {

		particleXComponent->addParticleEffect(ParticleEffects::deflect);

		//Get the Drone object
		auto droneObject = droneBrain->parent();

		//Play deflect sound
		const auto& soundComponent = droneObject.value()->getComponent<SoundComponent>(ComponentTypes::SOUND_COMPONENT);
		soundComponent->playSound("DEFLECT_SOUND");
	}

}

void MRContactListener::_playerBullet_enemyTurret(GameObject* playerBullet, GameObject* enemyTurret, b2Vec2 contactPoint)
{


	//Inflict damage to the turret
	const auto& turretVitalityComponent = enemyTurret->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);
	auto bulletVitality = playerBullet->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);

	auto bulletForce = bulletVitality->attackPower();

	bool turretDead = turretVitalityComponent->inflictDamage(bulletForce);
	if (turretDead) {

		enemyTurret->setRemoveFromWorld(true);

		//Add a particle Emitter for the explosion
		auto particleEmitterObject = SceneManager::instance().addGameObject("PARTICLE_X_EMITTER", GameLayer::MAIN, -1, -1);
		auto particleComponent = particleEmitterObject->getComponent<ParticleXComponent>(ComponentTypes::PARTICLE_X_COMPONENT);
		particleComponent->setType(ParticleEmitterType::ONETIME);
		contactPoint.x *= GameConfig::instance().scaleFactor();
		contactPoint.y *= GameConfig::instance().scaleFactor();
		particleEmitterObject->setPosition(contactPoint.x, contactPoint.y);

		//Lets make the turret crap same color as the turret, because it can be different at times
		auto turretScrap = ParticleEffects::turretScrap;
		turretScrap.setColorRangeBegin( enemyTurret->getColor());
		turretScrap.setColorRangeEnd(enemyTurret->getColor());
		particleComponent->addParticleEffect(turretScrap);

		//Add a smoke effect too
		particleComponent->addParticleEffect(ParticleEffects::explosionSmoke);

		//Play destroyed sound
		const auto& soundComponent = enemyTurret->getComponent<SoundComponent>(ComponentTypes::SOUND_COMPONENT);
		soundComponent->playSound("DESTROYED_SOUND");

	}
	else {

		const auto& soundComponent = enemyTurret->getComponent<SoundComponent>(ComponentTypes::SOUND_COMPONENT);
		soundComponent->playSound("IMPACTED_SOUND");

	}

	//Do a color flash animate on the turret
	const auto& turretAnimationComponent = enemyTurret->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);
	SDL_Color red = Colors::RED;
	turretAnimationComponent->setFlash(Colors::RED, .05, 3);

}

void MRContactListener::_player_wall(GameObject* player, GameObject* wall, b2Vec2 contactPoint)
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

void MRContactListener::_bullet_wall(GameObject* bullet, GameObject* wall, b2Vec2 contactPoint)
{

	//Build a One-Time particle emitter object
	auto particleEmitterObject = SceneManager::instance().addGameObject("PARTICLE_X_EMITTER", GameLayer::MAIN, -1, -1);
	const auto& particleComponent = particleEmitterObject->getComponent<ParticleComponent>(ComponentTypes::PARTICLE_X_COMPONENT);
	particleComponent->addParticleEffect(ParticleEffects::ricochet);

	//Convert from box2d to gameWorld coordinates
	contactPoint.x *= GameConfig::instance().scaleFactor();
	contactPoint.y *= GameConfig::instance().scaleFactor();
	particleEmitterObject->setPosition(contactPoint.x, contactPoint.y);

	//Set flag for removal for the Bullet
	bullet->setRemoveFromWorld(true);

}


void MRContactListener::_playerBullet_droneShield(GameObject* playerBullet, GameObject* droneShield, b2Vec2 contactPoint)
{


	auto particleEmitterObject = SceneManager::instance().addGameObject("PARTICLE_X_EMITTER", GameLayer::MAIN, -1, -1);
	auto particleComponent = particleEmitterObject->getComponent<ParticleXComponent>(ComponentTypes::PARTICLE_X_COMPONENT);

	particleComponent->setType(ParticleEmitterType::ONETIME);

	//Convert from box2d to gameWorld coordinates
	contactPoint.x *= GameConfig::instance().scaleFactor();
	contactPoint.y *= GameConfig::instance().scaleFactor();
	particleEmitterObject->setPosition(contactPoint.x, contactPoint.y);

	//Set flag for removal for the Bullet
	playerBullet->setRemoveFromWorld(true);

	//Test if the bullet is strong enought to destroy the shield piece
	auto bulletVitality = playerBullet->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);
	auto shieldVitality = droneShield->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);
	auto shieldHolds = shieldVitality->testResistance(bulletVitality->attackPower());

	//Sound component
	auto drone = droneShield->parent();
	const auto& soundComponent = drone.value()->getComponent<SoundComponent>(ComponentTypes::SOUND_COMPONENT);
	soundComponent->playSound("IMPACTED_SOUND");
	std::cout << "ImpactSound" << std::endl;

	if (shieldHolds == false) {

		particleComponent->addParticleEffect(ParticleEffects::impactSmoke);
		particleComponent->addParticleEffect(ParticleEffects::scrap);
		soundComponent->playSound("IMPACTED_SOUND");

	}
	else {

		particleComponent->addParticleEffect(ParticleEffects::deflect);
		soundComponent->playSound("DEFLECT_SOUND");
	}

}


void MRContactListener::_playerPulseBullet_droneShield(GameObject* playerPulseBullet, GameObject* droneShield, b2Vec2 contactPoint)
{


	//playerPulseBullet->setRemoveFromWorld(true);

	auto particleEmitterObject = SceneManager::instance().addGameObject("PARTICLE_X_EMITTER", GameLayer::MAIN, -1, -1);
	auto particleComponent = particleEmitterObject->getComponent<ParticleXComponent>(ComponentTypes::PARTICLE_X_COMPONENT);

	particleComponent->setType(ParticleEmitterType::ONETIME);
	//particleComponent->addParticleEffect(ParticleEffects::impactSmoke);
	//particleComponent->addParticleEffect(ParticleEffects::scrap);
	particleComponent->addParticleEffect(ParticleEffects::pulseExplosion);

	//Convert from box2d to gameWorld coordinates
	contactPoint.x *= GameConfig::instance().scaleFactor();
	contactPoint.y *= GameConfig::instance().scaleFactor();
	particleEmitterObject->setPosition(contactPoint.x, contactPoint.y);

	auto bulletVitality = playerPulseBullet->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);

	auto shieldVitality = droneShield->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);
	auto shieldHolds = shieldVitality->testResistance(bulletVitality->attackPower());

	//Get the drone onject itself object
	auto droneObject = droneShield->parent();
	const auto& soundComponent = droneObject.value()->getComponent<SoundComponent>(ComponentTypes::SOUND_COMPONENT);
	soundComponent->playSound("IMPACTED_SOUND");

	playerPulseBullet->setRemoveFromWorld(true);


}

void MRContactListener::_playerPulseBullet_wallPiece(GameObject* playerPulseBullet, GameObject* wallPiece, b2Vec2 contactPoint)
{

	auto particleEmitterObject = SceneManager::instance().addGameObject("PARTICLE_X_EMITTER", GameLayer::MAIN, -1, -1);
	auto particleComponent = particleEmitterObject->getComponent<ParticleXComponent>(ComponentTypes::PARTICLE_X_COMPONENT);

	particleComponent->setType(ParticleEmitterType::ONETIME);
	particleComponent->addParticleEffect(ParticleEffects::pulseExplosion);

	//Convert from box2d to gameWorld coordinates
	contactPoint.x *= GameConfig::instance().scaleFactor();
	contactPoint.y *= GameConfig::instance().scaleFactor();
	particleEmitterObject->setPosition(contactPoint.x, contactPoint.y);

	auto bulletVitality = playerPulseBullet->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);

	auto wallPieceVitality = wallPiece->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);

	playerPulseBullet->setRemoveFromWorld(true);

}
void MRContactListener::_playerPulseExplosion_wallPiece(GameObject* playerPulseExplosion, GameObject* wallPiece, b2Vec2 contactPoint)
{

	const auto& pulseBulletPhysics = playerPulseExplosion->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	const auto& wallVitality = wallPiece->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);

	const auto& soundComponent = wallPiece->parent().value()->getComponent<SoundComponent>(ComponentTypes::SOUND_COMPONENT);

	//Create a large pulse object
	auto shieldHolds = wallVitality->testResistance(5);
	playerPulseExplosion->setRemoveFromWorld(true);

	//Sound
	soundComponent->playSound("IMPACTED_SOUND");

	auto particleEmitterObject = SceneManager::instance().addGameObject("PARTICLE_X_EMITTER", GameLayer::MAIN, -1, -1);
	auto particleComponent = particleEmitterObject->getComponent<ParticleXComponent>(ComponentTypes::PARTICLE_X_COMPONENT);

	particleComponent->setType(ParticleEmitterType::ONETIME);

	//Convert from box2d to gameWorld coordinates
	contactPoint.x *= GameConfig::instance().scaleFactor();
	contactPoint.y *= GameConfig::instance().scaleFactor();
	particleEmitterObject->setPosition(contactPoint.x, contactPoint.y);

}

void MRContactListener::_playerPulseExplosion_droneShield(GameObject* playerPulseExplosion, GameObject* droneShield, b2Vec2 contactPoint)
{

	const auto& pulseBulletPhysics = playerPulseExplosion->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	const auto& shieldVitality = droneShield->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);

	//Create a large pulse object
	auto shieldHolds = shieldVitality->testResistance(5);
	playerPulseExplosion->setRemoveFromWorld(true);

	SoundManager::instance().playSound("SFX_IMPACT_1");

	auto particleEmitterObject = SceneManager::instance().addGameObject("PARTICLE_X_EMITTER", GameLayer::MAIN, -1, -1);
	auto particleComponent = particleEmitterObject->getComponent<ParticleXComponent>(ComponentTypes::PARTICLE_X_COMPONENT);

	particleComponent->setType(ParticleEmitterType::ONETIME);
	particleComponent->addParticleEffect(ParticleEffects::impactSmoke);
	particleComponent->addParticleEffect(ParticleEffects::scrap);

	//Convert from box2d to gameWorld coordinates
	contactPoint.x *= GameConfig::instance().scaleFactor();
	contactPoint.y *= GameConfig::instance().scaleFactor();
	particleEmitterObject->setPosition(contactPoint.x, contactPoint.y);


}

void MRContactListener::_playerBullet_wallPiece(GameObject* playerBullet, GameObject* wallPiece, b2Vec2 contactPoint)
{

	auto particleEmitterObject = SceneManager::instance().addGameObject("PARTICLE_X_EMITTER", GameLayer::MAIN, -1, -1);
	auto particleComponent = particleEmitterObject->getComponent<ParticleXComponent>(ComponentTypes::PARTICLE_X_COMPONENT);
	auto mainWallSoundComponent = wallPiece->parent().value()->getComponent<SoundComponent>(ComponentTypes::SOUND_COMPONENT);


	

	//auto particleEmitterObject = SceneManager::instance().addGameObject("PARTICLE_EMITTER", LAYER_MAIN, -1, -1);
	//auto particleComponent = particleEmitterObject->getComponent<ParticleComponent>(ComponentTypes::PARTICLE_COMPONENT);

	particleComponent->setType(ParticleEmitterType::ONETIME);
	//particleComponent->setType(ParticleEmitterType::CONTINUOUS);

	//Convert from box2d to gameWorld coordinates
	contactPoint.x *= GameConfig::instance().scaleFactor();
	contactPoint.y *= GameConfig::instance().scaleFactor();
	particleEmitterObject->setPosition(contactPoint.x, contactPoint.y);

	//Set flag for removal for the Bullet
	playerBullet->setRemoveFromWorld(true);

	//Test if the bullet is strong enought to destroy the shield piece
	auto bulletVitality = playerBullet->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);
	auto wallVitality = wallPiece->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);
	auto shieldHolds = wallVitality->testResistance(bulletVitality->attackPower());
	if (shieldHolds == false) {

		particleComponent->addParticleEffect(ParticleEffects::impactSmoke);
		mainWallSoundComponent->playSound("IMPACT_SOUND");

	}
	else {

		particleComponent->addParticleEffect(ParticleEffects::deflect);
		mainWallSoundComponent->playSound("DEFLECT_SOUND");
	}

}
void MRContactListener::_player_shieldScrap(GameObject* player, GameObject* shieldScrap, b2Vec2 contactPoint)
{

	//Acknowledge the scrap collection
	auto inventoryComponent = player->getComponent<InventoryComponent>(ComponentTypes::INVENTORY_COMPONENT);
	auto scrapCount = inventoryComponent->addCollectible(CollectibleTypes::DRONE_SCRAP, 1);

	//Update the status Manager
	game->contextMananger()->adjustStatusItemValue(StatusItemId::SCRAP_COUNT, 1);

	//Check to see if this upgrades the players weapon
	auto pistol = inventoryComponent->getItem(TraitTag::weapon);
	if (pistol.has_value()) {
		auto pistolWeaponComponent = pistol.value()->getComponent<PistolWeaponComponent>(ComponentTypes::WEAPON_COMPONENT);
		pistolWeaponComponent->checkLevelUp(scrapCount);
	}
	
	//flag the scrap item to be removed from the game and play a sound effect
	shieldScrap->setRemoveFromWorld(true);
	SoundManager::instance().playSound("SFX_PICKUP_2");

}

void MRContactListener::_enemyBullet_player(GameObject* bullet, GameObject* player, b2Vec2 contactPoint)
{

	//flag the scrap item to be removed from the game and play a sound effect
	bullet->setRemoveFromWorld(true);
	//SoundManager::instance().playSound("SFX_RETRO_IMPACT_5");

	//Do a color flash animate on the turret
	const auto& animationComponent = player->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);
	animationComponent->setFlash(Colors::RED, .05, 2);

	//Inflict damage
	const auto& playerVitalityComponent = player->getComponent<GinaVitalityComponent>(ComponentTypes::VITALITY_COMPONENT);
	const auto& bulletVitalityComponent = bullet->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);
	const auto& playerTransformComponent = bullet->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

	bool dead = playerVitalityComponent->inflictDamage(bulletVitalityComponent->attackPower());
	//bool dead = false;

	if (dead) {
		player->getComponent<PlayerControlComponent>(ComponentTypes::PLAYER_CONTROL_COMPONENT)->disable();
		player->disableCollision();
		player->disableRender();

		auto particleXEmitter = SceneManager::instance().addGameObject("PARTICLE_X_EMITTER", GameLayer::MAIN, -1, -1);
		auto particleXComponent = particleXEmitter->getComponent<ParticleXComponent>(ComponentTypes::PARTICLE_X_COMPONENT);

		particleXEmitter->setPosition(player->getCenterPosition());
		particleXComponent->addParticleEffect(ParticleEffects::explosionSmoke);
		particleXComponent->addParticleEffect(ParticleEffects::gibs);
		particleXComponent->addParticleEffect(ParticleEffects::playerExplode);

		const auto& soundComponent = player->getComponent<SoundComponent>(ComponentTypes::SOUND_COMPONENT);
		soundComponent->playSound("DESTROYED_SOUND");

	}


}

void MRContactListener::_radiationParticle_player(GameObject* radiationParticle, GameObject* player, b2Vec2 contactPoint)
{

	//flag the scrap item to be removed from the game and play a sound effect
	radiationParticle->setRemoveFromWorld(true);
	SoundManager::instance().playSound("SFX_RETRO_IMPACT_5");

	//Do a color flash animate on the turret
	const auto& animationComponent = player->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);
	animationComponent->setFlash(Colors::RED, .05, 2);

	//Inflict damage
	const auto& playerVitalityComponent = player->getComponent<GinaVitalityComponent>(ComponentTypes::VITALITY_COMPONENT);
	const auto& bulletVitalityComponent = radiationParticle->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);

	bool dead = playerVitalityComponent->inflictDamage(0.2);

	if (dead) {
		player->getComponent<PlayerControlComponent>(ComponentTypes::PLAYER_CONTROL_COMPONENT)->disable();
		player->disableCollision();
		player->disableRender();

		auto particleXEmitter = SceneManager::instance().addGameObject("PARTICLE_X_EMITTER", GameLayer::MAIN, -1, -1);
		auto particleXComponent = particleXEmitter->getComponent<ParticleXComponent>(ComponentTypes::PARTICLE_X_COMPONENT);

		particleXEmitter->setPosition(player->getCenterPosition());
		particleXComponent->addParticleEffect(ParticleEffects::explosionSmoke);
		particleXComponent->addParticleEffect(ParticleEffects::gibs);
		particleXComponent->addParticleEffect(ParticleEffects::playerExplode);

		SoundManager::instance().playSound("SFX_PLAYER_EXPLODE_1");
	}



}



void MRContactListener::_survivor_escape(GameObject* survivor, GameObject* escape, b2Vec2 contactPoint)
{

	//Update the status Manager
	game->contextMananger()->adjustStatusItemValue(StatusItemId::SURVIVORS, -1);

	//flag the scrap item to be removed from the game and play a sound effect
	survivor->setRemoveFromWorld(true);
	SoundManager::instance().playSound("SFX_PICKUP_2");

}

void MRContactListener::_player_heartPickup(GameObject* player, GameObject* heartPickup, b2Vec2 contactPoint)
{

	//player vitality
	const auto& playerVitality = player->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);

	if (playerVitality->addHealth(1)) {
		heartPickup->setRemoveFromWorld(true);
		SoundManager::instance().playSound("SFX_COLLECT_HEART_1");
	}



}


void MRContactListener::_player_checkpoint(GameObject* player, GameObject* checkpoint, b2Vec2 contactPoint)
{

	//contact component
	const auto& checkpointComponent = checkpoint->getComponent<CheckPointComponent>(ComponentTypes::CHECKPOINT_COMPONENT);

	checkpointComponent->setContactMade(true);


}

void MRContactListener::BeginContact(b2Contact* contact) {

	b2WorldManifold worldManifold;

	contact->GetWorldManifold(&worldManifold);
	b2Vec2 contactPoint = worldManifold.points[0];

	handleContact(contact, contactPoint);

}


void MRContactListener::EndContact(b2Contact* contact)
{


}



void MRContactListener::handleContact(b2Contact* contact, b2Vec2 contactPoint)
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

	//////////////////////////////////////////
	// Player Pulse  -  Wall Piece
	//////////////////////////////////////////
	if ((contactTag1 == ContactTag::PLAYER_PULSE_BULLET && contactTag2 == ContactTag::WALL_PIECE) ||
		(contactTag2 == ContactTag::PLAYER_PULSE_BULLET && contactTag1 == ContactTag::WALL_PIECE)) {

		if (contactTag1 == ContactTag::PLAYER_PULSE_BULLET) {
			_playerPulseBullet_wallPiece(contact1, contact2, contactPoint);
		}
		else {
			_playerPulseBullet_wallPiece(contact2, contact1, contactPoint);
		}
	}

	//////////////////////////////////////////
	// Player Pulse Exposion -  Wall Piece
	//////////////////////////////////////////
	if ((contactTag1 == ContactTag::PLAYER_PULSE_EXPLOSION && contactTag2 == ContactTag::WALL_PIECE) ||
		(contactTag2 == ContactTag::PLAYER_PULSE_EXPLOSION && contactTag1 == ContactTag::WALL_PIECE)) {

		if (contactTag1 == ContactTag::PLAYER_PULSE_EXPLOSION) {
			_playerPulseExplosion_wallPiece(contact1, contact2, contactPoint);
		}
		else {
			_playerPulseExplosion_wallPiece(contact2, contact1, contactPoint);
		}
	}

	//////////////////////////////////////////
	// Player Pulse Exposion -  Drone SHield
	//////////////////////////////////////////
	if ((contactTag1 == ContactTag::PLAYER_PULSE_EXPLOSION && contactTag2 == ContactTag::DRONE_SHIELD) ||
		(contactTag2 == ContactTag::PLAYER_PULSE_EXPLOSION && contactTag1 == ContactTag::DRONE_SHIELD)) {

		if (contactTag1 == ContactTag::PLAYER_PULSE_EXPLOSION) {
			_playerPulseExplosion_droneShield(contact1, contact2, contactPoint);
		}
		else {
			_playerPulseExplosion_droneShield(contact2, contact1, contactPoint);
		}
	}

	////////////////////////////////////
	// Player -  Heart Pickup
	//////////////////////////////////
	if ((contactTag1 == ContactTag::PLAYER_COLLISION && contactTag2 == ContactTag::HEART_PICKUP) ||
		(contactTag2 == ContactTag::PLAYER_COLLISION && contactTag1 == ContactTag::HEART_PICKUP)) {

		if (contactTag1 == ContactTag::PLAYER_COLLISION) {
			_player_heartPickup(contact1, contact2, contactPoint);
		}
		else {
			_player_heartPickup(contact2, contact1, contactPoint);
		}
	}

	//////////////////////////////////////
	// PlayerPulseBullet -  Drone Shield
	/////////////////////////////////////
	if ((contactTag1 == ContactTag::PLAYER_PULSE_BULLET && contactTag2 == ContactTag::DRONE_SHIELD) ||
		(contactTag2 == ContactTag::PLAYER_PULSE_BULLET && contactTag1 == ContactTag::DRONE_SHIELD)) {

		if (contactTag1 == ContactTag::PLAYER_PULSE_BULLET) {
			_playerPulseBullet_droneShield(contact1, contact2, contactPoint);
		}
		else {
			_playerPulseBullet_droneShield(contact2, contact1, contactPoint);
		}
	}

	////////////////////////////////////
	// PlayerBullet -  Drone Brain
	//////////////////////////////////
	if ((contactTag1 == ContactTag::PLAYER_BULLET && contactTag2 == ContactTag::DRONE_BRAIN) ||
		(contactTag2 == ContactTag::PLAYER_BULLET && contactTag1 == ContactTag::DRONE_BRAIN)) {

		if (contactTag1 == ContactTag::PLAYER_BULLET) {
			_playerBullet_droneBrain(contact1, contact2, contactPoint);
		}
		else {
			_playerBullet_droneBrain(contact2, contact1, contactPoint);
		}
	}

	////////////////////////////////////
	// PlayerBullet -  Enemy Turret
	//////////////////////////////////
	if ((contactTag1 == ContactTag::PLAYER_BULLET && contactTag2 == ContactTag::ENEMY_TURRET) ||
		(contactTag2 == ContactTag::PLAYER_BULLET && contactTag1 == ContactTag::ENEMY_TURRET)) {

		if (contactTag1 == ContactTag::PLAYER_BULLET) {
			_playerBullet_enemyTurret(contact1, contact2, contactPoint);
		}
		else {
			_playerBullet_enemyTurret(contact2, contact1, contactPoint);
		}
	}

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

	////////////////////////////////////
	// Survivor -  Escape Stairs
	//////////////////////////////////
	if ((contactTag1 == ContactTag::SURVIVOR && contactTag2 == ContactTag::ESCAPE_STAIRS) ||
		(contactTag2 == ContactTag::SURVIVOR && contactTag1 == ContactTag::ESCAPE_STAIRS)) {

		if (contactTag1 == ContactTag::SURVIVOR) {
			_survivor_escape(contact1, contact2, contactPoint);
		}
		else {
			_survivor_escape(contact2, contact1, contactPoint);
		}
	}

	////////////////////////////////////
	// Player Bullet -  WALL PIECE
	//////////////////////////////////
	if ((contactTag1 == ContactTag::PLAYER_BULLET && contactTag2 == ContactTag::WALL_PIECE) ||
		(contactTag2 == ContactTag::PLAYER_BULLET && contactTag1 == ContactTag::WALL_PIECE)) {

		if (contactTag1 == ContactTag::PLAYER_BULLET) {
			_playerBullet_wallPiece(contact1, contact2, contactPoint);
		}
		else {
			_playerBullet_wallPiece(contact2, contact1, contactPoint);
		}
	}

	////////////////////////////////////
	// Player Bullet -  Wall Contact
	//////////////////////////////////
	if ((contactTag1 == ContactTag::PLAYER_BULLET && contactTag2 == ContactTag::WALL) ||
		(contactTag2 == ContactTag::PLAYER_BULLET && contactTag1 == ContactTag::WALL)) {

		if (contactTag1 == ContactTag::PLAYER_BULLET) {
			_bullet_wall(contact1, contact2, contactPoint);
		}
		else {
			_bullet_wall(contact2, contact1, contactPoint);
		}
	}

	////////////////////////////////////
	// Enemy Bullet -  Wall Contact
	//////////////////////////////////
	if ((contactTag1 == ContactTag::ENEMY_BULLET && contactTag2 == ContactTag::WALL) ||
		(contactTag2 == ContactTag::ENEMY_BULLET && contactTag1 == ContactTag::WALL)) {

		if (contactTag1 == ContactTag::ENEMY_BULLET) {
			_bullet_wall(contact1, contact2, contactPoint);
		}
		else {
			_bullet_wall(contact2, contact1, contactPoint);
		}

	}

	///////////////////////////////////
	// Player Bullet -  Drone Shield
	///////////////////////////////////
	if ((contactTag1 == ContactTag::PLAYER_BULLET && contactTag2 == ContactTag::DRONE_SHIELD) ||
		(contactTag2 == ContactTag::PLAYER_BULLET && contactTag1 == ContactTag::DRONE_SHIELD)) {

		if (contactTag1 == ContactTag::PLAYER_BULLET) {
			_playerBullet_droneShield(contact1, contact2, contactPoint);
		}
		else {
			_playerBullet_droneShield(contact2, contact1, contactPoint);
		}

	}

	///////////////////////////////////
	// Player - Drone Scrap
	///////////////////////////////////
	if ((contactTag1 == ContactTag::PLAYER_COLLISION && contactTag2 == ContactTag::SHIELD_SCRAP) ||
		(contactTag2 == ContactTag::PLAYER_COLLISION && contactTag1 == ContactTag::SHIELD_SCRAP)) {

		if (contactTag1 == ContactTag::PLAYER_COLLISION) {
			_player_shieldScrap(contact1, contact2, contactPoint);
		}
		else {
			_player_shieldScrap(contact2, contact1, contactPoint);
		}

	}

	///////////////////////////////////
	// Enemy Bullet - Player
	///////////////////////////////////
	if ((contactTag1 == ContactTag::PLAYER_HITBOX && contactTag2 == ContactTag::ENEMY_BULLET) ||
		(contactTag2 == ContactTag::PLAYER_HITBOX && contactTag1 == ContactTag::ENEMY_BULLET)) {

		if (contactTag1 == ContactTag::ENEMY_BULLET) {
			_enemyBullet_player(contact1, contact2, contactPoint);
		}
		else {
			_enemyBullet_player(contact2, contact1, contactPoint);
		}

	}

	///////////////////////////////////
	// Radiation Particle - Player
	///////////////////////////////////
	if ((contactTag1 == ContactTag::PLAYER_HITBOX && contactTag2 == ContactTag::RADIATION_PARTICLE) ||
		(contactTag2 == ContactTag::PLAYER_HITBOX && contactTag1 == ContactTag::RADIATION_PARTICLE)) {

		if (contactTag1 == ContactTag::RADIATION_PARTICLE) {
			_radiationParticle_player(contact1, contact2, contactPoint);
		}
		else {
			_radiationParticle_player(contact2, contact1, contactPoint);
		}

	}

	///////////////////////////////////
	// Player - Checkpoint
	///////////////////////////////////
	if ((contactTag1 == ContactTag::PLAYER_COLLISION && contactTag2 == ContactTag::CHECKPOINT) ||
		(contactTag2 == ContactTag::PLAYER_COLLISION && contactTag1 == ContactTag::CHECKPOINT)) {

		if (contactTag1 == ContactTag::PLAYER_COLLISION) {
			_player_checkpoint(contact1, contact2, contactPoint);
		}
		else {
			_player_checkpoint(contact2, contact1, contactPoint);
		}

	}

}






