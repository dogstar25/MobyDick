#include "PhysicsSystem.h"
#include "../game.h"
#include "../components/TransformComponent.h"
#include "../components/PhysicsComponent.h"

void PhysicsSystem::Update()
{
	for (auto& entity : mEntities)
	{
		auto& transform = Game::instance().gameCoordinator().GetComponent<TransformComponent>(entity);
		auto& physics = Game::instance().gameCoordinator().GetComponent<PhysicsComponent>(entity);




	}


}
