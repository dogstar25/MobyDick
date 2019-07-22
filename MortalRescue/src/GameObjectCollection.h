#pragma once
#include <forward_list>
#include <list>
#include <memory>
#include <array>

#include "GameObject.h"
#include "ParticleObject.h"


class GameObjectCollection
{
public:
	GameObjectCollection();
	~GameObjectCollection();

	vector <unique_ptr<GameObject>> gameObjects;
	//list <ParticleObject*> particleObjects;
	//forward_list <ParticleObject*> particleObjects;
	vector <ParticleObject*> particleObjects;
	

};

