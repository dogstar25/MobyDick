#include "Coordinator.hpp"
#include "../EntityDefinitionManager.h"
#include "../components/RenderComponent.h"
#include "../components/TransformComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/AnimationComponent.h"




#include <json/json.h>


void Coordinator::addEntity(std::string entityDefinitionId, float xMapPos, float yMapPos, float angleAdjust)
{
	Json::Value definitionJSON;

	//Build components
	definitionJSON = EntityDefinitionManager::instance().getDefinition(entityDefinitionId)->definitionJSON();

	//Get an available entity
	auto entity = CreateEntity();

	//Always create a Transform component
	AddComponent(entity, TransformComponent(definitionJSON, xMapPos, yMapPos, angleAdjust));

	//Always create a Render component
	AddComponent(entity, RenderComponent(definitionJSON));

	//Physics Component
	if (definitionJSON.isMember("physicsComponent"))
	{
		AddComponent(entity, PhysicsComponent(definitionJSON, xMapPos, yMapPos, angleAdjust));

	}

	//Animation Component
	if (definitionJSON.isMember("animationComponent"))
	{
		AddComponent(entity, AnimationComponent(definitionJSON));

	}
}