#include "ContactFilter.h"

#include <assert.h>

#include "Globals.h"
#include "GameObject.h"

ContactFilter::ContactFilter()
{

	using namespace ObjectCategory;

	m_collisionMasks.resize(ObjectCategory::MAX_OBJECT_CATEGORIES);

	//Common
	m_collisionMasks[COMMON].reset();
	m_collisionMasks[COMMON].set(COLLECTABLE, true);
	m_collisionMasks[COMMON].set(COMMON, true);
	m_collisionMasks[COMMON].set(ENEMY_BULLET, true);
	m_collisionMasks[COMMON].set(ENEMY_FRAME, true);
	m_collisionMasks[COMMON].set(ENEMY_UNIT, true);
	m_collisionMasks[COMMON].set(FRIENDLY_BULLET, true);
	m_collisionMasks[COMMON].set(FRIENDLY_FRAME, true);
	m_collisionMasks[COMMON].set(FRIENDLY_UNIT, true);
	m_collisionMasks[COMMON].set(PLAYER, true);
	m_collisionMasks[COMMON].set(SMOKE_PARTICLE, true);
	m_collisionMasks[COMMON].set(SOLID_PARTICLE, true);
	m_collisionMasks[COMMON].set(WALL, true);

	//Collectable
	m_collisionMasks[COLLECTABLE].reset();
	m_collisionMasks[COLLECTABLE].set(COMMON, true);
	m_collisionMasks[COLLECTABLE].set(ENEMY_FRAME, true);
	m_collisionMasks[COLLECTABLE].set(ENEMY_UNIT, true);
	m_collisionMasks[COLLECTABLE].set(FRIENDLY_FRAME, true);
	m_collisionMasks[COLLECTABLE].set(PLAYER, true);
	m_collisionMasks[COLLECTABLE].set(WALL, true);

	//ENEMY_BULLET
	m_collisionMasks[ENEMY_BULLET].reset();
	m_collisionMasks[ENEMY_BULLET].set(COMMON, true);
	m_collisionMasks[ENEMY_BULLET].set(FRIENDLY_UNIT, true);
	m_collisionMasks[ENEMY_BULLET].set(WALL, true);

	//ENEMY_OBJECT_FRAME
	m_collisionMasks[ENEMY_FRAME].reset();
	m_collisionMasks[ENEMY_FRAME].set(COLLECTABLE, true);
	m_collisionMasks[ENEMY_FRAME].set(COMMON, true);
	m_collisionMasks[ENEMY_FRAME].set(FRIENDLY_FRAME, true);
	m_collisionMasks[ENEMY_FRAME].set(FRIENDLY_UNIT, true);
	m_collisionMasks[ENEMY_FRAME].set(PLAYER, true);
	m_collisionMasks[ENEMY_FRAME].set(SOLID_PARTICLE, true);
	m_collisionMasks[ENEMY_FRAME].set(WALL, true);

	//ENEMY_UNIT
	m_collisionMasks[ENEMY_UNIT].reset();
	m_collisionMasks[ENEMY_UNIT].set(COMMON, true);
	m_collisionMasks[ENEMY_UNIT].set(FRIENDLY_BULLET, true);
	m_collisionMasks[ENEMY_UNIT].set(FRIENDLY_UNIT, true);
	m_collisionMasks[ENEMY_UNIT].set(PLAYER, true);
	m_collisionMasks[ENEMY_UNIT].set(SOLID_PARTICLE, true);
	m_collisionMasks[ENEMY_UNIT].set(SOLID_PARTICLE, true);

	//FRIENDLY_BULLET
	m_collisionMasks[FRIENDLY_BULLET].reset();
	m_collisionMasks[FRIENDLY_BULLET].set(COMMON, true);
	m_collisionMasks[FRIENDLY_BULLET].set(ENEMY_UNIT, true);
	m_collisionMasks[FRIENDLY_BULLET].set(WALL, true);

	//FRIENDLY_OBJECT_FRAME
	m_collisionMasks[FRIENDLY_FRAME].reset();
	m_collisionMasks[FRIENDLY_FRAME].set(COLLECTABLE, true);
	m_collisionMasks[FRIENDLY_FRAME].set(COMMON, true);
	m_collisionMasks[FRIENDLY_FRAME].set(ENEMY_FRAME, true);
	m_collisionMasks[FRIENDLY_FRAME].set(ENEMY_UNIT, true);
	m_collisionMasks[FRIENDLY_FRAME].set(FRIENDLY_UNIT, true);
	m_collisionMasks[FRIENDLY_FRAME].set(PLAYER, true);
	m_collisionMasks[FRIENDLY_FRAME].set(SOLID_PARTICLE, true);
	m_collisionMasks[FRIENDLY_FRAME].set(WALL, true);

	//FRIENDLY_UNIT
	m_collisionMasks[FRIENDLY_UNIT].reset();
	m_collisionMasks[FRIENDLY_UNIT].set(COMMON, true);
	m_collisionMasks[FRIENDLY_UNIT].set(ENEMY_BULLET, true);
	m_collisionMasks[FRIENDLY_UNIT].set(ENEMY_FRAME, true);
	m_collisionMasks[FRIENDLY_UNIT].set(ENEMY_UNIT, true);
	m_collisionMasks[FRIENDLY_UNIT].set(FRIENDLY_FRAME, true);
	m_collisionMasks[FRIENDLY_UNIT].set(FRIENDLY_UNIT, true);
	m_collisionMasks[FRIENDLY_UNIT].set(PLAYER, true);
	m_collisionMasks[FRIENDLY_UNIT].set(SOLID_PARTICLE, true);
	m_collisionMasks[FRIENDLY_UNIT].set(WALL, true);

	//PLAYER
	m_collisionMasks[PLAYER].reset();
	m_collisionMasks[PLAYER].set(COLLECTABLE, true);
	m_collisionMasks[PLAYER].set(COMMON, true);
	m_collisionMasks[PLAYER].set(ENEMY_BULLET, true);
	m_collisionMasks[PLAYER].set(ENEMY_FRAME, true);
	m_collisionMasks[PLAYER].set(ENEMY_UNIT, true);
	m_collisionMasks[PLAYER].set(FRIENDLY_FRAME, true);
	m_collisionMasks[PLAYER].set(FRIENDLY_UNIT, true);
	m_collisionMasks[PLAYER].set(SOLID_PARTICLE, true);
	m_collisionMasks[PLAYER].set(WALL, true);

	//SMOKE_PARTICLE
	m_collisionMasks[SMOKE_PARTICLE].reset();

	//SOLID_PARTICLE
	m_collisionMasks[SOLID_PARTICLE].reset();
	m_collisionMasks[SOLID_PARTICLE].set(COMMON, true);
	m_collisionMasks[SOLID_PARTICLE].set(ENEMY_UNIT, true);
	m_collisionMasks[PLAYER].set(PLAYER, true);
	m_collisionMasks[PLAYER].set(WALL, true);

	//WALL
	m_collisionMasks[WALL].reset();
	m_collisionMasks[WALL].set(COLLECTABLE, true);
	m_collisionMasks[WALL].set(COMMON, true);
	m_collisionMasks[WALL].set(ENEMY_BULLET, true);
	m_collisionMasks[WALL].set(ENEMY_FRAME, true);
	m_collisionMasks[WALL].set(ENEMY_UNIT, true);
	m_collisionMasks[WALL].set(FRIENDLY_BULLET, true);
	m_collisionMasks[WALL].set(FRIENDLY_FRAME, true);
	m_collisionMasks[WALL].set(FRIENDLY_UNIT, true);
	m_collisionMasks[WALL].set(PLAYER, true);
	m_collisionMasks[WALL].set(SOLID_PARTICLE, true);



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

	auto gameObjectAMask = m_collisionMasks[gameObjectA->m_category];
	auto gameObjectBMask = m_collisionMasks[gameObjectB->m_category];

	if (gameObjectAMask.test(gameObjectB->m_category) ||
		gameObjectBMask.test(gameObjectA->m_category)) {
		return true;
	}
	else {
		return false;
	}

}
