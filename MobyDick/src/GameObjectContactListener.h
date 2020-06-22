#ifndef GAME_OBJECT_CONTACT_LISTENER_H
#define GAME_OBJECT_CONTACT_LISTENER_H

#include <Box2D/Box2D.h>

class PlayerObject;
class WorldObject;

class GameObjectContactListener : public b2ContactListener
{
public:
	void BeginContact(b2Contact*);
	void EndContact(b2Contact*);

	void handleContact(WorldObject* contact1, WorldObject* contact2, b2Vec2 contactPoint);
	void bulletWall(WorldObject* bullet, WorldObject* wall, b2Vec2 contactPoint);
	void bulletPiece(WorldObject* bullet, WorldObject* wall, b2Vec2 contactPoint);
	void bulletPieceExplode(WorldObject* bullet, WorldObject* wall, b2Vec2 contactPoint);
	void bulletPieceDeflect(WorldObject* bullet, WorldObject* wall, b2Vec2 contactPoint);
	void playerBitPiece(PlayerObject* player, WorldObject* bitPiece, b2Vec2 contactPoint);

	
};

#endif

