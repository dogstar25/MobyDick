#include "ContactListener.h"
#include "SceneManager.h"

#include <iostream>

#include "game.h"


void ContactListener::BeginContact(b2Contact* contact) {


	//Get the 2 object pointers
	GameObject* gameObjectA = static_cast<GameObject*>(contact->GetFixtureA()->GetBody()->GetUserData());
	GameObject* gameObjectB = static_cast<GameObject*>(contact->GetFixtureB()->GetBody()->GetUserData());

	float x = 0;
	float y = 0;
	b2WorldManifold worldManifold;

	contact->GetWorldManifold(&worldManifold);
	b2Vec2 contactPoint = worldManifold.points[0];

	if (gameObjectA != nullptr && gameObjectB != nullptr)
	{
		handleContact(gameObjectA, gameObjectB, contactPoint);
	}

}

void ContactListener::EndContact(b2Contact* contact)
{
	

}

void ContactListener::handleContact(GameObject* contact1, GameObject* contact2, b2Vec2 contactPoint)
{

	std::cout<< "contact was made!";

}






