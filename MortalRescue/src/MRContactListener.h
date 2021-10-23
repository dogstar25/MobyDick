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

	void virtual handleContact(GameObject* contact1, GameObject* contact2, b2Vec2 contactPoint);

private:
	void _player_wall(GameObject* contact1, GameObject* contact2, b2Vec2 contactPoint);
	void _bullet_wall(GameObject* contact1, GameObject* contact2, b2Vec2 contactPoint);
	void _playerBullet_droneShield(GameObject* contact1, GameObject* contact2, b2Vec2 contactPoint);
	void _player_shieldScrap(GameObject* contact1, GameObject* contact2, b2Vec2 contactPoint);
};

#endif

