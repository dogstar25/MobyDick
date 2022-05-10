#ifndef CONTACT_LISTENER_H
#define CONTACT_LISTENER_H

#include <string>
#include <vector>
#include <array>

#include <box2d/box2d.h>

class GameObject;


class ContactListener : public b2ContactListener
{
public:
	ContactListener() = default;

	//static ContactListener& instance();

	//Override functions
	void virtual BeginContact(b2Contact*) override;
	void virtual EndContact(b2Contact*) override;

	void virtual handleBeginContact(GameObject* contact1, GameObject* contact2, b2Vec2 contactPoint);
	void virtual handleEndContact(GameObject* contact1, GameObject* contact2, b2Vec2 contactPoint);

private:
	
};

#endif

