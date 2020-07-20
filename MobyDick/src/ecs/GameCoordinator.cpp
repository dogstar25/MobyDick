#include "GameCoordinator.h"

#include "../components/TransformComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/RenderComponent.h"

#include "../systems/PhysicsSystem.h"
#include "../systems/RenderSystem.h"


GameCoordinator::GameCoordinator()
{

}

GameCoordinator::~GameCoordinator()
{

}

void GameCoordinator::init()
{
	Coordinator::init();

	RegisterComponent<TransformComponent>();
	RegisterComponent<PhysicsComponent>();
	RegisterComponent<RenderComponent>();
	RegisterComponent<AnimationComponent>();

	RegisterSystem<PhysicsSystem>();
	RegisterSystem<RenderSystem>();

}
