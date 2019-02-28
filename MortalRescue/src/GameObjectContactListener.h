#pragma once
#include <Box2D/Box2D.h>

class GameObjectContactListener : public b2ContactListener
{
public:
	void BeginContact(b2Contact*);
	void EndContact(b2Contact*);
};

