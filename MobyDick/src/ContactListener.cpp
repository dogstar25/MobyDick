#include "ContactListener.h"
#include "SceneManager.h"


void ContactListener::BeginContact(b2Contact* contact) {


	//Get the 2 object pointers
	GameObject* gameObjectA = reinterpret_cast<GameObject*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
	GameObject* gameObjectB = reinterpret_cast<GameObject*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

	b2WorldManifold worldManifold;

	contact->GetWorldManifold(&worldManifold);
	b2Vec2 contactPoint = worldManifold.points[0];

	if (gameObjectA != nullptr && gameObjectB != nullptr)
	{
		handleBeginContact(gameObjectA, gameObjectB, contactPoint);
	}

}

void ContactListener::EndContact(b2Contact* contact)
{

	GameObject* gameObjectA = reinterpret_cast<GameObject*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
	GameObject* gameObjectB = reinterpret_cast<GameObject*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

	b2WorldManifold worldManifold;

	contact->GetWorldManifold(&worldManifold);
	b2Vec2 contactPoint = worldManifold.points[0];

	if (gameObjectA != nullptr && gameObjectB != nullptr)
	{
		handleEndContact(gameObjectA, gameObjectB, contactPoint);
	}


}

void ContactListener::handleBeginContact(GameObject* contact1, GameObject* contact2, b2Vec2 contactPoint)
{

	contact1->addTouchingObject(contact2);
	contact2->addTouchingObject(contact1);

}

void ContactListener::handleEndContact(GameObject* contact1, GameObject* contact2, b2Vec2 contactPoint)
{

	contact1->removeTouchingObject(contact2);
	contact2->removeTouchingObject(contact1);

}






