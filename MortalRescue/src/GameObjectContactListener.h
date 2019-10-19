#pragma once
#include <Box2D/Box2D.h>
#include "PlayerObject.h"

class GameObjectContactListener : public b2ContactListener
{
public:
	void BeginContact(b2Contact*);
	void EndContact(b2Contact*);

	b2Vec2 findWallImpactPoint(b2Vec2 worldPoint, PlayerObject* player);
};

