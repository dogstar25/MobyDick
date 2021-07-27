#include "MRComponentFactory.h"
#include "components/BrainComponent.h"
#include "components/InventoryComponent.h"
#include "components/DroneBrainComponent.h"


std::shared_ptr<Component> MRComponentFactory::_create(
	Json::Value definitionJSON,
	std::string textComponentGameObjectid,
	Scene* scene,
	float xMapPos,
	float yMapPos,
	float angleAdjust,
	ComponentTypes componentType)
{
	std::shared_ptr<Component> component{};

	auto gameObjectId = definitionJSON["id"].asString();

	//Handle game specific componenets, otherwise call the base ComponentFactory _create
	if (gameObjectId == "DRONE" && componentType == ComponentTypes::BRAIN_COMPONENT) {
		component = std::make_shared<DroneBrainComponent>(definitionJSON);
	}
	else {
		component = ComponentFactory::_create(
			definitionJSON,
			textComponentGameObjectid,
			scene,
			xMapPos,
			yMapPos,
			angleAdjust,
			componentType);
	}

    return component;
}
