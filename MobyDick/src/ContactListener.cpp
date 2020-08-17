#include "ContactListener.h"

#include <iostream>

#include "Globals.h"
#include "GameObject.h"


ContactListener::ContactListener()
{

	m_ObjectCategoryLabels[ObjectCategory::COLLECTABLE] = ObjectCategoryLabel::COLLECTABLE;
	m_ObjectCategoryLabels[ObjectCategory::COMMON] = ObjectCategoryLabel::COMMON;
	m_ObjectCategoryLabels[ObjectCategory::ENEMY_BULLET] = ObjectCategoryLabel::ENEMY_BULLET;
	m_ObjectCategoryLabels[ObjectCategory::ENEMY_FRAME] = ObjectCategoryLabel::ENEMY_FRAME;
	m_ObjectCategoryLabels[ObjectCategory::ENEMY_UNIT] = ObjectCategoryLabel::ENEMY_UNIT;
	m_ObjectCategoryLabels[ObjectCategory::FRIENDLY_BULLET] = ObjectCategoryLabel::ENEMY_FRAME;
	m_ObjectCategoryLabels[ObjectCategory::FRIENDLY_UNIT] = ObjectCategoryLabel::FRIENDLY_UNIT;
	m_ObjectCategoryLabels[ObjectCategory::PLAYER] = ObjectCategoryLabel::PLAYER;
	m_ObjectCategoryLabels[ObjectCategory::SMOKE_PARTICLE] = ObjectCategoryLabel::SMOKE_PARTICLE;
	m_ObjectCategoryLabels[ObjectCategory::SOLID_PARTICLE] = ObjectCategoryLabel::SOLID_PARTICLE;
	m_ObjectCategoryLabels[ObjectCategory::WALL] = ObjectCategoryLabel::WALL;

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
	auto category1 = contact1->category();
	auto category2 = contact2->category();
	std::string collisionActionClass = {};

	if (m_ObjectCategoryLabels[category1] < m_ObjectCategoryLabels[category2]) {
		collisionActionClass = { m_ObjectCategoryLabels[category1] + "_" + m_ObjectCategoryLabels[category2] };
	}
	else {
		collisionActionClass = { m_ObjectCategoryLabels[category2] + "_" + m_ObjectCategoryLabels[category1] };
	}
	
	std::cout << "Class is " << collisionActionClass << "\n";

	/*
	CollisionAction?
	EventAction?
	WorldAction
	CollisionEvent
	ContactEvent - I like this one
	ContactAction
	ContactActivity
	
	These actions would be handled by some other class (collisionManager?)
	It will pass both objects and the contact point to the function
	The contactAction would execute, and then ..maybe...we call onCollision of each of the objects 
	passing in the other object it hit to do object specific stuff.
	It could have a onCOllision action built specifically for it, using the actionManager actionClass framework

	*/

	int todd = 1;
	//if (contact1->definition()->id.compare("BULLET1") == 0 ||
	//	contact2->definition()->id.compare("BULLET1") == 0 ||
	//	contact1->definition()->id.compare("BULLET2") == 0 ||
	//	contact2->definition()->id.compare("BULLET2") == 0)
	//{
	//	//Bullet Wall contact
	//	if (contact1->definition()->id.compare(0, 4, "WALL") == 0 ||
	//		contact2->definition()->id.compare(0, 4, "WALL") == 0)
	//	{
	//		if (contact1->definition()->id.compare("BULLET1") == 0 ||
	//			contact1->definition()->id.compare("BULLET2") == 0)
	//		{
	//			bullet = contact1;
	//			wall = contact2;
	//		}
	//		else if (contact2->definition()->id.compare("BULLET1") == 0 ||
	//				 contact2->definition()->id.compare("BULLET2") == 0) 
	//		{
	//			bullet = contact2;
	//			wall = contact1;
	//		}

	//		this->bulletWall(bullet, wall, contactPoint);
	//	}

	//	//Bullet Piece contact
	//	if (contact1->definition()->id.compare(5, 5, "PIECE") == 0 ||
	//		contact1->definition()->id.compare(7, 5, "PIECE") == 0 ||
	//		contact2->definition()->id.compare(5, 5, "PIECE") == 0 ||
	//		contact2->definition()->id.compare(7, 5, "PIECE") == 0)
	//	{
	//		if (contact1->definition()->id.compare("BULLET1") == 0 ||
	//			contact1->definition()->id.compare("BULLET2") == 0) {
	//			bullet = contact1;
	//			piece = contact2;
	//		}
	//		else if (contact2->definition()->id.compare("BULLET1") == 0 ||
	//				 contact2->definition()->id.compare("BULLET2") == 0) 
	//		{
	//			bullet = contact2;
	//			piece = contact1;
	//		}

	//		this->bulletPiece(bullet, piece, contactPoint);

	//	}

	//}
	//if (contact1->definition()->id.compare("GINA_64") == 0 ||
	//	contact2->definition()->id.compare("GINA_64") == 0)
	//{
	//	//Bullet Wall contact
	//	if (contact1->definition()->id.compare("ANGLE_BIT_PIECE") == 0 ||
	//		contact2->definition()->id.compare("ANGLE_BIT_PIECE") == 0)
	//	{
	//		if (contact1->definition()->id.compare("GINA_64") == 0) {
	//			player = dynamic_cast<PlayerObject*>(contact1);
	//			piece = contact2;
	//		}
	//		else if (contact2->definition()->id.compare("GINA_64") == 0) {
	//			player = dynamic_cast<PlayerObject*>(contact2);
	//			piece = contact1;
	//		}

	//		this->playerBitPiece(player, piece, contactPoint);


	//	}
	//}


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
//	if (piece->testStrength(bullet->strength())) {
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





