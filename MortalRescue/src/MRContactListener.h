#ifndef MR_CONTACT_LISTENER_H
#define MR_CONTACT_LISTENER_H

#include <string>
#include <vector>
#include <array>

#include <box2d/box2d.h>

#include "ContactListener.h"

class GameObject;


class MRContactListener : public ContactListener
{
public:

	MRContactListener() {};

	void BeginContact(b2Contact*) override;
	void EndContact(b2Contact*) override;

	void virtual handleContact(b2Contact* contact, b2Vec2 contactPoint) override;

private:

	void _player_wall(GameObject*, GameObject*, b2Vec2 contactPoint);
	void _bullet_wall(GameObject*, GameObject*, b2Vec2 contactPoint);
	void _playerBullet_droneShield(GameObject*, GameObject*, b2Vec2 contactPoint);
	void _player_shieldScrap(GameObject*, GameObject*, b2Vec2 contactPoint);
	void _enemyBullet_player(GameObject*, GameObject*, b2Vec2 contactPoint);
	void _radiationParticle_player(GameObject*, GameObject*, b2Vec2 contactPoint);
	void _survivor_escape(GameObject*, GameObject*, b2Vec2 contactPoint);
	void _playerBullet_wallPiece(GameObject*, GameObject*, b2Vec2 contactPoint);
	void _playerBullet_enemyTurret(GameObject*, GameObject*, b2Vec2 contactPoint);
	void _playerBullet_droneBrain(GameObject*, GameObject*, b2Vec2 contactPoint);
	void _player_heartPickup(GameObject*, GameObject*, b2Vec2 contactPoint);
	void _playerPulseBullet_droneShield(GameObject*, GameObject*, b2Vec2 contactPoint);
	void _playerPulseBullet_wallPiece(GameObject*, GameObject*, b2Vec2 contactPoint);
	void _playerPulseExplosion_droneShield(GameObject*, GameObject*, b2Vec2 contactPoint);
	void _playerPulse_wallPiece(GameObject*, GameObject*, b2Vec2 contactPoint);
	void _playerPulseExplosion_wallPiece(GameObject*, GameObject*, b2Vec2 contactPoint);
	void _player_checkpoint(GameObject*, GameObject*, b2Vec2 contactPoint);
	
};

#endif

