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
	Coordinator::Init();

	RegisterComponent<TransformComponent>();
	RegisterComponent<PhysicsComponent>();
	RegisterComponent<RenderComponent>();

	RegisterSystem<PhysicsSystem>();
	RegisterSystem<RenderSystem>();

}
