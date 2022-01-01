#pragma once
#include "components/Component.h"
#include "ComponentFactory.h"
#include <json/json.h>

class Scene;

class MRComponentFactory : public ComponentFactory
{

public:


private:
	//Create version that has ALL possible inputs
	std::shared_ptr<Component> _create(
		Json::Value definitionJSON,
		std::string textComponentGameObjectid,
		Scene* scene,
		float xMapPos,
		float yMapPos,
		float angleAdjust,
		ComponentTypes componentType) override;

};

