#include "..\..\MobyDick\src\ComponentFactory.h"
#include "..\..\MobyDick\src\ComponentFactory.h"
#include "..\..\MobyDick\src\ComponentFactory.h"
#include "..\..\MobyDick\src\ComponentFactory.h"
#include "..\..\MobyDick\src\ComponentFactory.h"
#include "ComponentFactory.h"
#include "components/BrainComponent.h"
#include "components/InventoryComponent.h"


ComponentFactory& ComponentFactory::instance()
{
	static ComponentFactory singletonInstance;
	return singletonInstance;
}

std::shared_ptr<Component> ComponentFactory::create(Json::Value definitionJSON, ComponentTypes componentType)
{
	std::shared_ptr<Component> component{};
	component = _create(definitionJSON, "", nullptr, 0, 0, 0, 0, componentType);
	return component;
}

std::shared_ptr<Component> ComponentFactory::create(Json::Value definitionJSON, int layer, ComponentTypes componentType)
{
	std::shared_ptr<Component> component{};
	component = _create(definitionJSON, "", nullptr, 0, 0, 0, layer, componentType);
	return component;
}

std::shared_ptr<Component> ComponentFactory::create(Json::Value definitionJSON, Scene* scene, ComponentTypes componentType)
{
	std::shared_ptr<Component> component{};
	component = _create(definitionJSON, "", scene, 0, 0, 0, 0, componentType);
	return component;
}

std::shared_ptr<Component> ComponentFactory::create(Json::Value definitionJSON, std::string textGameObjectId, ComponentTypes componentType)
{
	std::shared_ptr<Component> component{};
	component = _create(definitionJSON, textGameObjectId, nullptr, 0, 0, 0, 0, componentType);
	return component;
}

std::shared_ptr<Component> ComponentFactory::create(Json::Value definitionJSON, float xMapPos, float yMapPos, float angleAdjust, ComponentTypes componentType)
{
	std::shared_ptr<Component> component{};
	component = _create(definitionJSON, "", nullptr, xMapPos, yMapPos, angleAdjust, 0, componentType);
	return component;
}

std::shared_ptr<Component> ComponentFactory::create(Json::Value definitionJSON, Scene* scene, float xMapPos, float yMapPos, float angleAdjust, ComponentTypes componentType)
{
	std::shared_ptr<Component> component{};
	component = _create(definitionJSON, "", scene, xMapPos, yMapPos, angleAdjust, 0, componentType);
	return component;

}

std::shared_ptr<Component> ComponentFactory::_create(
	Json::Value definitionJSON,
	std::string textComponentGameObjectid,
	Scene* scene,
	float xMapPos,
	float yMapPos,
	float angleAdjust,
	int layer,
	ComponentTypes componentType)
{
	std::shared_ptr<Component> component{};

	auto gameObjectId = definitionJSON["id"].asString();

	switch (componentType) {

		case ComponentTypes::ANIMATION_COMPONENT:
				component = std::make_shared<AnimationComponent>(definitionJSON);
			break;
		case ComponentTypes::ACTION_COMPONENT:
				component = std::make_shared<ActionComponent>(definitionJSON);
			break;
		case ComponentTypes::ATTACHMENTS_COMPONENT:
			component = std::make_shared<AttachmentsComponent>(definitionJSON, scene);
			break;
		case ComponentTypes::BRAIN_COMPONENT:
			component = std::make_shared<BrainComponent>(definitionJSON);
			break;
		case ComponentTypes::CHILDREN_COMPONENT:
			component = std::make_shared<ChildrenComponent>(definitionJSON, scene);
			break;
		case ComponentTypes::COMPOSITE_COMPONENT:
			component = std::make_shared<CompositeComponent>(definitionJSON, scene);
			break;
		case ComponentTypes::INVENTORY_COMPONENT:
			component = std::make_shared<InventoryComponent>(definitionJSON, scene);
			break;
		case ComponentTypes::NAVIGATION_COMPONENT:
			component = std::make_shared<NavigationComponent>(definitionJSON);
			break;
		case ComponentTypes::PARTICLE_COMPONENT:
			component = std::make_shared<ParticleComponent>(definitionJSON);
			break;
		case ComponentTypes::PARTICLE_X_COMPONENT:
			component = std::make_shared<ParticleXComponent>(definitionJSON);
			break;
		case ComponentTypes::PHYSICS_COMPONENT:
			component = std::make_shared<PhysicsComponent>(definitionJSON, scene, xMapPos, yMapPos, angleAdjust);
			break;
		case ComponentTypes::PLAYERCONTROL_COMPONENT:
			component = std::make_shared<PlayerControlComponent>(definitionJSON);
			break;
		case ComponentTypes::POOL_COMPONENT:
			component = std::make_shared<PoolComponent>(definitionJSON);
			break;
		case ComponentTypes::RENDER_COMPONENT:
			component = std::make_shared<RenderComponent>(definitionJSON, layer);
			break;
		case ComponentTypes::TEXT_COMPONENT:
			component = std::make_shared<TextComponent>(textComponentGameObjectid, definitionJSON);
			break;
		case ComponentTypes::TRANSFORM_COMPONENT:
			component = std::make_shared<TransformComponent>(definitionJSON, xMapPos, yMapPos, angleAdjust);
			break;
		case ComponentTypes::UICONTROL_COMPONENT:
			component = std::make_shared<UIControlComponent>(definitionJSON);
			break;
		case ComponentTypes::VITALITY_COMPONENT:
			component = std::make_shared<VitalityComponent>(definitionJSON);
			break;
		case ComponentTypes::WEAPON_COMPONENT:
			component = std::make_shared<WeaponComponent>(definitionJSON);
			break;

	}

    return component;
}
