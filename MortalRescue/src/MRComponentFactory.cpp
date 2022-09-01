#include "MRComponentFactory.h"

#include "GameConstants.h"
#include "components/BrainComponent.h"
#include "components/InventoryComponent.h"
#include "components/DroneBrainComponent.h"
#include "components/PistolWeaponComponent.h"
#include "components/TurretBrainComponent.h"
#include "components/SurvivorBrainComponent.h"
#include "components/GinaPlayerControlComponent.h"
#include "components/GinaVitalityComponent.h"

std::shared_ptr<Component> MRComponentFactory::create(
	Json::Value definitionJSON,
	std::string gameObjectName,
	std::string gameObjectTextType,
	Scene* scene,
	float xMapPos,
	float yMapPos,
	float angleAdjust,
	const int componentType)
{
	std::shared_ptr<Component> component{};
	auto gameObjectType = definitionJSON["type"].asString();
	Json::Value componentJSON{};

	//Handle game specific componenets, otherwise call the base ComponentFactory 'create'
	if (componentType == ComponentTypes::BRAIN_DRONE_COMPONENT) {
		componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::BRAIN_DRONE_COMPONENT);
		component = std::make_shared<DroneBrainComponent>(componentJSON);
	}
	else if (componentType == ComponentTypes::WEAPON_PISTOL_COMPONENT) {
		componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::WEAPON_PISTOL_COMPONENT);
		component = std::make_shared<PistolWeaponComponent>(componentJSON);
	}
	else if (componentType == ComponentTypes::TURRET_BRAIN_COMPONENT) {
		componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::TURRET_BRAIN_COMPONENT);
		component = std::make_shared<TurretBrainComponent>(componentJSON);
	}
	else if (componentType == ComponentTypes::SURVIVOR_BRAIN_COMPONENT) {
		componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::SURVIVOR_BRAIN_COMPONENT);
		component = std::make_shared<SurvivorBrainComponent>(componentJSON);
	}
	else if (componentType == ComponentTypes::GINA_PLAYER_CONTROL_COMPONENT) {
		componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::GINA_PLAYER_CONTROL_COMPONENT);
		component = std::make_shared<GinaPlayerControlComponent>(componentJSON);
	}
	else if (componentType == ComponentTypes::GINA_VITALITY_COMPONENT) {
		componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::GINA_VITALITY_COMPONENT);
		component = std::make_shared<GinaVitalityComponent>(componentJSON);
	}
	
	else {
		component = ComponentFactory::create(
			definitionJSON,
			gameObjectName,
			gameObjectTextType,
			scene,
			xMapPos,
			yMapPos,
			angleAdjust,
			componentType);
	}

    return component;
}
