#include "CollisionManager.h"



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

