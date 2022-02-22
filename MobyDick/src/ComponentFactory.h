#pragma once
#include "components/Component.h"
#include <json/json.h>

class Scene;

class ComponentFactory
{


public:

	ComponentFactory() = default;

	//static ComponentFactory& instance();
	//std::shared_ptr<Component> create(Json::Value definitionJSON, const int componentType);
	//std::shared_ptr<Component> create(Json::Value definitionJSON, Scene* scene, const int componentType);
	//std::shared_ptr<Component> create(Json::Value definitionJSON, std::string textGameObjectId, const int componentType);
	//std::shared_ptr<Component> create(
	//	Json::Value definitionJSON, float xMapPos, float yMapPos,float angleAdjust, const int componentType);
	//std::shared_ptr<Component> create(
	//	Json::Value definitionJSON, Scene* scene, float xMapPos, float yMapPos,	float angleAdjust, const int componentType);

	//Create version that has ALL possible inputs
	virtual std::shared_ptr<Component> create(
		Json::Value definitionJSON,
		std::string gameObjectName,
		std::string textComponentGameObjectid,
		Scene* scene,
		float xMapPos,
		float yMapPos,
		float angleAdjust,
		const int componentType);

};

