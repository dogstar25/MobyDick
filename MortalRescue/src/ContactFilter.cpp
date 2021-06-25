#include "ContactFilter.h"

#include "GameConstants.h"
#include "GameObject.h"

ContactFilter::ContactFilter()
{

	using namespace CollisionTag;

	m_collisionMasks.resize(CollisionTag::MAX_OBJECT_CATEGORIES);

	//General_solid - set all to collide
	m_collisionMasks[GENERAL_SOLID].reset();
	m_collisionMasks[GENERAL_SOLID].flip();

	//General_free - set all to NOT collide
	m_collisionMasks[GENERAL_FREE].reset();

	//Intangible - set all to intangle
	m_collisionMasks[DEFLECT_EFFECT].reset();
	m_collisionMasks[SMOKE_PARTICLE].reset();

	//Non-stationary
	//DRONE_BRAIN
	m_collisionMasks[DRONE_BRAIN].reset();
	m_collisionMasks[DRONE_BRAIN].set(WALL);

	//DRONE_FRAME
	m_collisionMasks[DRONE_FRAME].reset();
	m_collisionMasks[DRONE_FRAME].set(HEAVY_PARTICLE);
	m_collisionMasks[DRONE_FRAME].set(PLAYER);
	m_collisionMasks[DRONE_FRAME].set(SHIELD_SCRAP);
	m_collisionMasks[DRONE_FRAME].set(SURVIVOR);
	m_collisionMasks[DRONE_FRAME].set(WALL);
	m_collisionMasks[DRONE_FRAME].set(DRONE_FRAME);

	//DRONE_SHIELD
	m_collisionMasks[DRONE_SHIELD].reset();
	m_collisionMasks[DRONE_SHIELD].set(HEAVY_PARTICLE);
	m_collisionMasks[DRONE_SHIELD].set(LIGHT_PARTICLE);
	m_collisionMasks[DRONE_SHIELD].set(PLAYER_BULLET);

	//DRONE_WEAPON
	m_collisionMasks[DRONE_WEAPON].reset();
	m_collisionMasks[DRONE_WEAPON].set(FRIENDLY_BULLET);
	m_collisionMasks[DRONE_WEAPON].set(HEAVY_PARTICLE);
	m_collisionMasks[DRONE_WEAPON].set(PLAYER);
	m_collisionMasks[DRONE_WEAPON].set(PLAYER_BULLET);
	m_collisionMasks[DRONE_WEAPON].set(SURVIVOR);
	m_collisionMasks[DRONE_WEAPON].set(WALL);

	//FRIENDLY_BULLET
	m_collisionMasks[FRIENDLY_BULLET].reset();
	m_collisionMasks[FRIENDLY_BULLET].set(WALL);

	//ENEMY_BULLET
	m_collisionMasks[ENEMY_BULLET].reset();
	m_collisionMasks[ENEMY_BULLET].set(PLAYER);
	m_collisionMasks[ENEMY_BULLET].set(SURVIVOR);
	m_collisionMasks[ENEMY_BULLET].set(WALL);

	//HEAVY_PARTICLE
	m_collisionMasks[HEAVY_PARTICLE].reset();
	m_collisionMasks[HEAVY_PARTICLE].set(WALL);

	//LIGHT_PARTICLE
	m_collisionMasks[LIGHT_PARTICLE].reset();
	m_collisionMasks[LIGHT_PARTICLE].set(WALL);

	//PLAYER
	m_collisionMasks[PLAYER].reset();
	m_collisionMasks[PLAYER].set(SHIELD_SCRAP);
	m_collisionMasks[PLAYER].set(SURVIVOR);
	m_collisionMasks[PLAYER].set(DRONE_FRAME);
	m_collisionMasks[PLAYER].set(WALL);

	//PLAYER_BULLET
	m_collisionMasks[PLAYER_BULLET].reset();
	m_collisionMasks[PLAYER_BULLET].set(DRONE_SHIELD);
	m_collisionMasks[PLAYER_BULLET].set(WALL);

	//SHIELD_SCRAP
	m_collisionMasks[SHIELD_SCRAP].reset();
	m_collisionMasks[SHIELD_SCRAP].set(PLAYER);
	m_collisionMasks[SHIELD_SCRAP].set(DRONE_FRAME);
	m_collisionMasks[SHIELD_SCRAP].set(WALL);

	//SURVIVOR
	m_collisionMasks[SURVIVOR].reset();
	m_collisionMasks[SURVIVOR].set(WALL);

	//WALL
	m_collisionMasks[WALL].reset();
	m_collisionMasks[WALL].set(GENERAL_SOLID);
	m_collisionMasks[WALL].set(DRONE_FRAME);
	m_collisionMasks[WALL].set(FRIENDLY_BULLET);
	m_collisionMasks[WALL].set(HEAVY_PARTICLE);
	m_collisionMasks[WALL].set(PLAYER);
	m_collisionMasks[WALL].set(PLAYER_BULLET);
	m_collisionMasks[WALL].set(SHIELD_SCRAP);
	m_collisionMasks[WALL].set(SURVIVOR);
	m_collisionMasks[WALL].set(SURVIVOR);

}


ContactFilter::~ContactFilter()
{

}

ContactFilter& ContactFilter::instance()
{
	static ContactFilter singletonInstance;
	return singletonInstance;

}


bool ContactFilter::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
{

	if (fixtureA == nullptr || fixtureB == nullptr) {
		return false;
	}

	GameObject* gameObjectA = static_cast<GameObject*>(fixtureA->GetBody()->GetUserData());
	GameObject* gameObjectB = static_cast<GameObject*>(fixtureB->GetBody()->GetUserData());

	if (gameObjectA == nullptr || gameObjectB == nullptr) {
		return false;
	}

	auto gameObjectAMask = m_collisionMasks[gameObjectA->collisionTag()];
	auto gameObjectBMask = m_collisionMasks[gameObjectB->collisionTag()];

	//ToDo:this OR should be an AND
	if (gameObjectAMask.test(gameObjectB->collisionTag()) &&
		gameObjectBMask.test(gameObjectA->collisionTag())) {
		return true;
	}
	else {
		return false;
	}

}
