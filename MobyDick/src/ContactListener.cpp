#include "ContactListener.h"

#include <iostream>

#include "game.h"


ContactListener::ContactListener()
{

	//m_ObjectCategoryLabels[IdTag::COLLECTABLE] = IdTag::COLLECTABLE;
	//m_ObjectCategoryLabels[ObjectCategory::COMMON] = ObjectCategoryLabel::COMMON;
	//m_ObjectCategoryLabels[ObjectCategory::ENEMY_BULLET] = ObjectCategoryLabel::ENEMY_BULLET;
	//m_ObjectCategoryLabels[ObjectCategory::ENEMY_FRAME] = ObjectCategoryLabel::ENEMY_FRAME;
	//m_ObjectCategoryLabels[ObjectCategory::ENEMY_UNIT] = ObjectCategoryLabel::ENEMY_UNIT;
	//m_ObjectCategoryLabels[ObjectCategory::FRIENDLY_BULLET] = ObjectCategoryLabel::ENEMY_FRAME;
	//m_ObjectCategoryLabels[ObjectCategory::FRIENDLY_UNIT] = ObjectCategoryLabel::FRIENDLY_UNIT;
	//m_ObjectCategoryLabels[ObjectCategory::PLAYER] = ObjectCategoryLabel::PLAYER;
	//m_ObjectCategoryLabels[ObjectCategory::SMOKE_PARTICLE] = ObjectCategoryLabel::SMOKE_PARTICLE;
	//m_ObjectCategoryLabels[ObjectCategory::SOLID_PARTICLE] = ObjectCategoryLabel::SOLID_PARTICLE;
	//m_ObjectCategoryLabels[ObjectCategory::WALL] = ObjectCategoryLabel::WALL;

}

ContactListener::~ContactListener()
{

}

ContactListener& ContactListener::instance()
{

	static ContactListener singletonInstance;
	return singletonInstance;

}


void ContactListener::BeginContact(b2Contact* contact) {


	//Get the 2 object pointers
	GameObject* gameObjectA = static_cast<GameObject*>(contact->GetFixtureA()->GetBody()->GetUserData());
	GameObject* gameObjectB = static_cast<GameObject*>(contact->GetFixtureB()->GetBody()->GetUserData());

	float x = 0;
	float y = 0;
	b2WorldManifold worldManifold;

	contact->GetWorldManifold(&worldManifold);
	b2Vec2 contactPoint = worldManifold.points[0];

	if (gameObjectA != nullptr && gameObjectB != nullptr)
	{
		handleContact(gameObjectA, gameObjectB, contactPoint);
	}

}


void ContactListener::EndContact(b2Contact* contact)
{
	

}



void ContactListener::handleContact(GameObject* contact1, GameObject* contact2, b2Vec2 contactPoint)
{
	auto category1 = contact1->idTag();
	auto category2 = contact2->idTag();
	std::string collisionActionClass = {};

	//if (m_ObjectCategoryLabels[category1] < m_ObjectCategoryLabels[category2]) {
	//	collisionActionClass = { m_ObjectCategoryLabels[category1] + "_" + m_ObjectCategoryLabels[category2] };
	//}
	//else {
	//	collisionActionClass = { m_ObjectCategoryLabels[category2] + "_" + m_ObjectCategoryLabels[category1] };
	//}

	//std::cout << "Class is " << collisionActionClass << "\n";

	/////////////////////////
	// Player Wall Contact
	////////////////////////
	if ((category1 == IdTag::PLAYER && category2 == IdTag::WALL) ||
		(category2 == IdTag::PLAYER && category1 == IdTag::WALL)) {

		player_wall(contact1, contact2, contactPoint);

	}

	////////////////////////////////////
	// Player Bullet -  Wall Contact
	//////////////////////////////////
	if ((category1 == IdTag::PLAYER_BULLET && category2 == IdTag::WALL) ||
		(category2 == IdTag::PLAYER_BULLET && category1 == IdTag::WALL)) {

		bullet_wall(contact1, contact2, contactPoint);

	}

	///////////////////////////////////
	// Player Bullet -  Drone Shield
	///////////////////////////////////
	if ((category1 == IdTag::PLAYER_BULLET && category2 == IdTag::DRONE_SHIELD) ||
		(category2 == IdTag::PLAYER_BULLET && category1 == IdTag::DRONE_SHIELD)) {

		playerBullet_droneShield(contact1, contact2, contactPoint);

	}

}

void ContactListener::player_wall(GameObject* contact1, GameObject* contact2, b2Vec2 contactPoint)
{
	GameObject* player;
	GameObject* wall;

	if (contact1->idTag() == IdTag::PLAYER) {
		player = contact1;
		wall = contact2;
	}
	else {
		player = contact2;
		wall = contact1;
	}

	std::cout << "player_wall called\n";


}

void ContactListener::bullet_wall(GameObject* contact1, GameObject* contact2, b2Vec2 contactPoint)
{
	GameObject* bullet;
	GameObject* wall;

	if (contact1->idTag() == IdTag::PLAYER_BULLET) {
		bullet = contact1;
		wall = contact2;
	}
	else {
		bullet = contact2;
		wall = contact1;
	}

	//Build a One-Time particle emitter object
	auto particleEmitterObject = Game::instance().addGameObject("PARTICLE_EMITTER", LAYER_MAIN, -1, -1);
	auto& particleComponent = particleEmitterObject->getComponent<ParticleComponent>(ComponentTypes::PARTICLE_COMPONENT);
	particleComponent->addParticleEffect(ParticleEffects::ricochet);
	particleComponent->setType(ParticleEmitterType::ONETIME);

	/*auto particleEmitterObject = Game::instance().addGameObject("PARTICLE_EMITTER", LAYER_MAIN, -1, -1);
	auto& particleComponent = particleEmitterObject->getComponent<ParticleComponent>(ComponentTypes::PARTICLE_COMPONENT);
	particleComponent->addParticleEffect(ParticleEffects::ricochet);
	particleComponent->setType(ParticleEmitterType::ONETIME);
	particleComponent->setEmissionInterval(std::chrono::duration<float>(0.2));*/

	//Convert from box2d to gameWorld coordinates
	contactPoint.x *= GameConfig::instance().scaleFactor();
	contactPoint.y *= GameConfig::instance().scaleFactor();
	particleEmitterObject->setPosition(contactPoint.x, contactPoint.y);

	//Set flag for removal for the Bullet
	bullet->setRemoveFromWorld(true);

}

void ContactListener::playerBullet_droneShield(GameObject* contact1, GameObject* contact2, b2Vec2 contactPoint)
{
	GameObject* bullet;
	GameObject* shield;

	if (contact1->idTag() == IdTag::PLAYER_BULLET) {
		bullet = contact1;
		shield = contact2;
	}
	else {
		bullet = contact2;
		shield = contact1;
	}


	auto particleEmitterObject = Game::instance().addGameObject("PARTICLE_X_EMITTER", LAYER_BACKGROUND, -1, -1);
	auto& particleComponent = particleEmitterObject->getComponent<ParticleXComponent>(ComponentTypes::PARTICLE_X_COMPONENT);
	particleComponent->setType(ParticleEmitterType::ONETIME);

	//Convert from box2d to gameWorld coordinates
	contactPoint.x *= GameConfig::instance().scaleFactor();
	contactPoint.y *= GameConfig::instance().scaleFactor();
	particleEmitterObject->setPosition(contactPoint.x, contactPoint.y);

	//Set flag for removal for the Bullet
	bullet->setRemoveFromWorld(true);

	//Test if the bullet is strong enought to destroy the shield piece
	auto& bulletVitality = bullet->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);
	auto& shieldVitality = shield->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);
	auto shieldHolds = shieldVitality->testResistance(bulletVitality->attackPower());
	if (shieldHolds == false) {

		shieldVitality->setIsBroken(true);
		particleComponent->addParticleEffect(ParticleEffects::ricochetX);
		particleComponent->addParticleEffect(ParticleEffects::scrap);

	}
	else {

		particleComponent->addParticleEffect(ParticleEffects::deflect);
	}






}
//void ContactListener::playerBitPiece(PlayerObject* player, WorldObject* piece, b2Vec2 contactPoint)
//{
//	//Set flag for bullet to be removed from world
//	piece->setRemoveFromWorld(true);
//	player->incrementPiecesCollected();
//
//	//Sound
//	SoundManager::instance().playSound("SFX_PICKUP_2");
//
//
//}
//
//
//void ContactListener::bulletWall(WorldObject* bullet, WorldObject* wall, b2Vec2 contactPoint)
//{
//	//Set flag for bullet to be removed from world
//	bullet->setRemoveFromWorld(true);
//
//	//use the collision point for the particle emission
//	float x = contactPoint.x;
//	float y = contactPoint.y;
//	b2Vec2 particleOrigin = { x,y };
//
//	//temp color code
//	SDL_Color colorMin = { 0,0,0,255 };
//	SDL_Color colorMax = { 225,255,255,255 };
//
//
//	ParticleEmission* particleEmission = new ParticleEmission(
//		"PARTICLE1_POOL",
//		particleOrigin, //min position
//		particleOrigin,	//max position
//		5,	//Force Min
//		5,	//force Max
//		0.55,	//Lifetime Min
//		1.55,	//Lifetime Max
//		true,	// Alpha fade
//		0,	//Angle min
//		360,	//Angle Max
//		0.28,	//Size Min
//		1.48,	//Size Max
//		colorMin,	//Color Min
//		colorMax,	//Color Max
//		40,	//Particle count min
//		50	//Particle count max
//	);
//	ParticleMachine::instance().add(particleEmission);
//
//}
//
//void ContactListener::bulletPiece(WorldObject* bullet, WorldObject* piece, b2Vec2 contactPoint)
//{
//
//
//	//Set flag for bullet to be removed from world
//	bullet->setRemoveFromWorld(true);
//
//	//Set flag for piece to be removed from world
//	//only if bullet was strong enough
//	if (piece->testStrength(bullet->force())) {
//		piece->setRemoveFromWorld(true);
//		bulletPieceExplode(bullet,piece,contactPoint);
//	}
//	else
//	{
//		bulletPieceDeflect(bullet, piece, contactPoint);
//	}
//	
//
//
//}
//
//void ContactListener::bulletPieceExplode(WorldObject* bullet, WorldObject* piece, b2Vec2 contactPoint)
//{
//	ParticleEmission* particleEmission = NULL;
//
//	float x = contactPoint.x;
//	float y = contactPoint.y;
//	b2Vec2 particleOrigin = { x,y };
//
//	SDL_Color colorMin = piece->color();
//	SDL_Color colorMax = piece->color();
//
//	particleEmission = new ParticleEmission(
//		"PARTICLE1_POOL",
//		particleOrigin, //min position
//		particleOrigin,	//max position
//		1,	//Force Min
//		10,	//force Max
//		0.35,	//Lifetime Min
//		0.65,	//Lifetime Max
//		true,	// Alpha fade
//		0,	//Angle min
//		360,	//Angle Max
//		1.28,	//Size Min
//		3.48,	//Size Max
//		colorMin,	//Color Min
//		colorMax,	//Color Max
//		10,	//Particle count min
//		25	//Particle count max
//	);
//	ParticleMachine::instance().add(particleEmission);
//
//	//Create some white smoke particles
//	colorMin = { 255,255,255,50 };
//	colorMax = { 255,255,255,100 };
//
//	particleEmission = new ParticleEmission(
//		"PARTICLE1_POOL",
//		particleOrigin, //min position
//		particleOrigin,	//max position
//		1,	//Force Min
//		8,	//force Max
//		0.55,	//Lifetime Min
//		0.75,	//Lifetime Max
//		false,	// Alpha fade
//		0,	//Angle min
//		360,	//Angle Max
//		2.00,	//Size Min
//		5.25,	//Size Max
//		colorMin,	//Color Min
//		colorMax,	//Color Max
//		1,	//Particle count min
//		8	//Particle count max
//	);
//	ParticleMachine::instance().add(particleEmission);
//
//	//Also emit 2 pieces that will remain on teh ground
//	colorMin = piece->color();
//	colorMax = piece->color();
//	particleEmission = new ParticleEmission(
//		"PIECES1_POOL",
//		particleOrigin, //min position
//		particleOrigin,	//max position
//		20,	//Force Min
//		75,	//force Max
//		0,	//Lifetime Min
//		0,	//Lifetime Max
//		false,	// Alpha fade
//		0,	//Angle min
//		360,	//Angle Max
//		0.16,	//Size Min
//		0.16,	//Size Max
//		colorMin,	//Color Min
//		colorMax,	//Color Max
//		2,	//Particle count min
//		3	//Particle count max
//	);
//	ParticleMachine::instance().add(particleEmission);
//
//	//Sound
//	SoundManager::instance().playSound("SFX_IMPACT_1");
//
//}
//
//void ContactListener::bulletPieceDeflect(WorldObject* bullet, WorldObject* piece, b2Vec2 contactPoint)
//{
//	ParticleEmission* particleEmission = NULL;
//
//	float x = contactPoint.x;
//	float y = contactPoint.y;
//	b2Vec2 particleOrigin = { x,y };
//
//	SDL_Color colorMin = { 255,255,255,175 };
//	SDL_Color colorMax = { 255,255,255,175 };
//
//	particleEmission = new ParticleEmission(
//		"DEFLECT1_POOL",
//		particleOrigin, //min position
//		particleOrigin,	//max position
//		2,	//Force Min
//		3,	//force Max
//		0.10,	//Lifetime Min
//		0.25,	//Lifetime Max
//		true,	// Alpha fade
//		0,	//Angle min
//		360,	//Angle Max
//		2.56,	//Size Min
//		2.56,	//Size Max
//		colorMin,	//Color Min
//		colorMax,	//Color Max
//		2,	//Particle count min
//		3	//Particle count max
//	);
//	ParticleMachine::instance().add(particleEmission);
//
//	//Sound
//	SoundManager::instance().playSound("SFX_IMPACT_3");
//
//
//}





