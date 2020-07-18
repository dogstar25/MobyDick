#include "Coordinator.hpp"
#include "../EntityDefinitionManager.h"
#include "../components/RenderComponent.h"
#include "../components/TransformComponent.h"

#include <json/json.h>

void Coordinator::addEntity(std::string entityDefinitionId, float xMapPos, float yMapPos, float angleAdjust)
{
	Json::Value definitionJSON;

	//Build components
	definitionJSON = EntityDefinitionManager::instance().getDefinition(entityDefinitionId)->definitionJSON();

	//Always create a Transform component
	auto entity = CreateEntity();
	AddComponent(entity, TransformComponent(definitionJSON, xMapPos, yMapPos, angleAdjust));

	//Always create a Render component
	auto entity = CreateEntity();
	AddComponent(entity, RenderComponent(definitionJSON));

	//Animation Component
	/*if (definitionJSON.isMember("animationComponent"))
	{
		auto entity = CreateEntity();
		AddComponent(entity, AnimationComponent(definitionJSON));

	}*/

}