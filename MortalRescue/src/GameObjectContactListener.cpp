#include "GameObjectContactListener.h"
#include "GameObject.h"
#include "WorldObject.h"



void GameObjectContactListener::BeginContact(b2Contact* contact) {

	//check if fixture A was a ball
	WorldObject* gameObjectA = static_cast<WorldObject*>(contact->GetFixtureA()->GetBody()->GetUserData());
	if (gameObjectA)
	{
		//static_cast<GameObject*>(bodyUserData)->startContact();
	}

	//check if fixture B was a ball
	WorldObject* gameObjectB = static_cast<WorldObject*>(contact->GetFixtureB()->GetBody()->GetUserData());
	if (gameObjectB)
	{
		//static_cast<GameObject*>(bodyUserData)->startContact();
		//gameObjectB->physicsBody->SetActive(false);
		//gameObjectB->physicsBody->SetAwake(false);
		
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

