#include "GameObjectContactListener.h"
#include "GameObject.h"



void GameObjectContactListener::BeginContact(b2Contact* contact) {

	//check if fixture A was a ball
	GameObject* gameObjectA = static_cast<GameObject*>(contact->GetFixtureA()->GetBody()->GetUserData());
	if (gameObjectA)
	{
		//static_cast<GameObject*>(bodyUserData)->startContact();
	}

	//check if fixture B was a ball
	GameObject* gameObjectB = static_cast<GameObject*>(contact->GetFixtureB()->GetBody()->GetUserData());
	if (gameObjectB)
	{
		//static_cast<GameObject*>(bodyUserData)->startContact();
	}



}

void GameObjectContactListener::EndContact(b2Contact* contact)
{

	//check if fixture A was a ball
	GameObject* bodyUserDataA = static_cast<GameObject*>(contact->GetFixtureA()->GetBody()->GetUserData());
	if (bodyUserDataA)
	{
		//static_cast<GameObject*>(bodyUserData)->startContact();
	}

	//check if fixture B was a ball
	GameObject* bodyUserDataB = static_cast<GameObject*>(contact->GetFixtureB()->GetBody()->GetUserData());
	if (bodyUserDataB)
	{
		//static_cast<GameObject*>(bodyUserData)->startContact();
	}

}

