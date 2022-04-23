#include "ContactFilter.h"

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

}

bool ContactFilter::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
{

	if (fixtureA == nullptr || fixtureB == nullptr) {
		return false;
	}

	GameObject* gameObjectA = reinterpret_cast<GameObject*>(fixtureA->GetBody()->GetUserData().pointer);
	GameObject* gameObjectB = reinterpret_cast<GameObject*>(fixtureB->GetBody()->GetUserData().pointer);

	if (gameObjectA == nullptr || gameObjectB == nullptr) {
		return false;
	}

	auto gameObjectAMask = m_collisionMasks[gameObjectA->collisionTag()];
	auto gameObjectBMask = m_collisionMasks[gameObjectB->collisionTag()];

	if (gameObjectAMask.test(gameObjectB->collisionTag()) &&
		gameObjectBMask.test(gameObjectA->collisionTag())) {

		return true;
	}
	else {
		return false;
	}

}
