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

//	static MRContactListener& instance();

	//Override functions
	void BeginContact(b2Contact*) override;
	void EndContact(b2Contact*) override;

	void virtual handleContact(b2Contact* contact, b2Vec2 contactPoint) override;

private:
	void _bullet_wall(GameObject* bullet, GameObject* wall, b2Vec2 contactPoint);
	void _playerBullet_droneShield(GameObject* playerBullet, GameObject* droneShield, b2Vec2 contactPoint);
	void _player_shieldScrap(GameObject* player, GameObject* shieldScrap, b2Vec2 contactPoint);
	void _enemyBullet_player(GameObject* bullet, GameObject* player, b2Vec2 contactPoint);
};

#endif

