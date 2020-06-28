#ifndef GAME_OBJECT_COLLECTION_H
#define GAME_OBJECT_COLLECTION_H

#include <vector>

class GameObject;
class ParticleObject;

class GameObjectCollection
{
public:
	GameObjectCollection();
	~GameObjectCollection();

	//Accessor functions
	std::vector <GameObject*>& gameObjects() {
		return m_gameObjects;
	}
	std::vector <ParticleObject*>& particleObjects() {
		return m_particleObjects;
	}

private:

	//We keep our particle objects in a seperate collection because they have a limited lifetime and 
	//that collection goes through a heavy amount of additions and deletions
	std::vector <GameObject*> m_gameObjects;
	std::vector <ParticleObject*> m_particleObjects;
	

};

#endif
