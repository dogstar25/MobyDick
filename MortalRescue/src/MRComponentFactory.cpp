#include "MRComponentFactory.h"

#include "GameConstants.h"
#include "components/BrainComponent.h"
#include "components/InventoryComponent.h"
#include "components/DroneBrainComponent.h"
#include "components/PistolWeaponComponent.h"


std::shared_ptr<Component> MRComponentFactory::create(
	Json::Value definitionJSON,
	std::string textComponentGameObjectid,
	Scene* scene,
	float xMapPos,
	float yMapPos,
	float angleAdjust,
	const int componentType)
{
	std::shared_ptr<Component> component{};

	auto gameObjectId = definitionJSON["id"].asString();


	//Handle game specific componenets, otherwise call the base ComponentFactory 'create'
	if (componentType == ComponentTypes::BRAIN_DRONE_COMPONENT) {
		component = std::make_shared<DroneBrainComponent>(definitionJSON);
	}
	else if (componentType == ComponentTypes::WEAPON_PISTOL_COMPONENT) {
		component = std::make_shared<PistolWeaponComponent>(definitionJSON);
	}
	else {
		component = ComponentFactory::create(
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
