#include "GameCoordinator.h"

#include "../components/TransformComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/RenderComponent.h"
#include "../components/ActionComponent.h"
#include "../components/PlayerControlComponent.h"
#include "../components/VitalityComponent.h"

#include "../systems/PhysicsSystem.h"
#include "../systems/RenderSystem.h"
#include "../systems/AnimationSystem.h"
#include "../systems/PlayerControlSystem.h"


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
	RegisterComponent<PlayerControlComponent>();
	RegisterComponent<VitalityComponent>();
	RegisterComponent<ActionComponent>();

	RegisterSystem<PhysicsSystem>();
	RegisterSystem<RenderSystem>();
	RegisterSystem<AnimationSystem>();
	RegisterSystem<PlayerControlSystem>();

}
