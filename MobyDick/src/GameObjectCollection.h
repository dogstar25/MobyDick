#pragma once
#include <forward_list>
#include <list>
#include <memory>
#include <vector>

#include "GameObject.h"
#include "ParticleObject.h"


class GameObjectCollection
{
public:
	GameObjectCollection();
	~GameObjectCollection();

	//We keep our particle objects in a seperate collection because they have a limited lifetime and 
	//that collection goes through a heavy amount of additions and deletions
	std::vector <GameObject*> gameObjects;
	std::vector <ParticleObject*> particleObjects;
	

};

