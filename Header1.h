#pragma once

class GameObject
{
public:

	GameObject();
	virtual ~GameObject();

	int x, y;

}

class PlayerObject : Public GameObject
{

	PlayerObject();
	virtual ~PlayerObject();

	int x, y;
	string name;

};


class Game
{

	unique_ptr<GameObject> randomObject;
	unique_ptr<PlayerObject> player;


}

#########################



unique_ptr<GameObject> GameObjectManager::buildGameObject(string gameObjectId)
{

	unique_ptr<GameObject> gameObject;
	gameObject = make_unique< GameObject>();

	return gameObject;

}


##################################
int main(int argc, char *args[])

{


	unique_ptr<GameObject> rock = gameObjectManager.buildGameObject("ROCK"));
	unique_ptr<PlayerObject> playerObject = dynamic_cast<unique_ptr<PlayerObject>>(Game::gameObjectManager.buildGameObject("GINA_64", 5, 5));



}


