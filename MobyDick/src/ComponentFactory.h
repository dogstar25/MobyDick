#pragma once
#include "components/Component.h"
#include <jsoncpp/include/json/json.h>

class Scene;

class ComponentFactory
{


public:

	static ComponentFactory& instance();
	std::shared_ptr<Component> create(Json::Value definitionJSON, ComponentTypes componentType);
	std::shared_ptr<Component> create(Json::Value definitionJSON, int layer, ComponentTypes componentType);
	std::shared_ptr<Component> create(Json::Value definitionJSON, Scene* scene, ComponentTypes componentType);
	std::shared_ptr<Component> create(Json::Value definitionJSON, std::string textGameObjectId, ComponentTypes componentType);
	std::shared_ptr<Component> create(
		Json::Value definitionJSON, float xMapPos, float yMapPos,float angleAdjust, ComponentTypes componentType);
	std::shared_ptr<Component> create(
		Json::Value definitionJSON, Scene* scene, float xMapPos, float yMapPos,	float angleAdjust, ComponentTypes componentType);

protected:
	//Create version that has ALL possible inputs
	virtual std::shared_ptr<Component> _create(
		Json::Value definitionJSON,
		std::string textComponentGameObjectid,
		Scene* scene,
		float xMapPos,
		float yMapPos,
		float angleAdjust,
		int layer,
		ComponentTypes componentType);

};

