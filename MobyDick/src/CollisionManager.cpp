#include "CollisionManager.h"

#include <assert.h>

#include "Globals.h"
#include "GameObject.h"

CollisionManager::CollisionManager()
{


}

CollisionManager::~CollisionManager()
{

}

CollisionManager& CollisionManager::instance()
{
	static CollisionManager singletonInstance;
	return singletonInstance;

}

