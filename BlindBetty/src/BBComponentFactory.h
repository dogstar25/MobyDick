#pragma once
#include "components/Component.h"
#include "ComponentFactory.h"
#include <json/json.h>

class Scene;

class BBComponentFactory : public ComponentFactory
{

public:


private:
	//Create version that has ALL possible inputs
	std::shared_ptr<Component> create(
		Json::Value definitionJSON,
		std::string gameObjectName,
		std::string gameObjectTextType,
		Scene* scene,
		float xMapPos,
		float yMapPos,
		float angleAdjust,
		const int componentType) override;

};

