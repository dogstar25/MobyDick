#include "RenderSystem.h"

#include "../game.h"
#include "../components/TransformComponent.h"
#include "../components/PhysicsComponent.h"

void RenderSystem::update()
{
	for (auto& entity : mEntities)
	{
		auto& transform = Game::instance().gameCoordinator().GetComponent<TransformComponent>(entity);
		auto& physics = Game::instance().gameCoordinator().GetComponent<PhysicsComponent>(entity);




	}


}

void RenderSystem::render()
{

}

void RenderSystem::init()
{
	/*
	Build the component signature for this system
	*/
	Signature signature;
	signature.set(Game::instance().gameCoordinator().GetComponentType<TransformComponent>());

	Game::instance().gameCoordinator().SetSystemSignature<RenderSystem>(signature);
}
