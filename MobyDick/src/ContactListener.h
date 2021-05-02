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
	ContactListener();
	~ContactListener();

	static ContactListener& instance();
	void BeginContact(b2Contact*);
	void EndContact(b2Contact*);
	void handleContact(GameObject* contact1, GameObject* contact2, b2Vec2 contactPoint);

private:
	std::array<std::string, 32> m_ObjectCategoryLabels;
	
};

#endif

