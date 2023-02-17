#ifndef BB_CONTACT_LISTENER_H
#define BB_CONTACT_LISTENER_H

#include <string>
#include <vector>
#include <array>

#include <box2d/box2d.h>

#include "ContactListener.h"

class GameObject;


class BBContactListener : public ContactListener
{
public:

	BBContactListener() {};

	void BeginContact(b2Contact*) override;
	void EndContact(b2Contact*) override;

	void virtual handleContact(b2Contact* contact, b2Vec2 contactPoint) override;

private:

	void _actor_warpEntry(GameObject*, GameObject*, b2Vec2 contactPoint);
	
};

#endif

