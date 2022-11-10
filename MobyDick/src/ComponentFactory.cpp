#include "ComponentFactory.h"


std::shared_ptr<Component> ComponentFactory::create(
	Json::Value definitionJSON,
	std::string name,
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

	switch (componentType) {

		case ComponentTypes::ANIMATION_COMPONENT:
			componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::ANIMATION_COMPONENT);
			component = std::make_shared<AnimationComponent>(componentJSON);
			break;
		case ComponentTypes::ACTION_COMPONENT:
			componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::ACTION_COMPONENT);
			component = std::make_shared<ActionComponent>(componentJSON, scene);
			break;
		case ComponentTypes::ATTACHMENTS_COMPONENT:
			componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::ATTACHMENTS_COMPONENT);
			component = std::make_shared<AttachmentsComponent>(componentJSON, name, scene);
			break;
		case ComponentTypes::BRAIN_COMPONENT:
			componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::BRAIN_COMPONENT);
			component = std::make_shared<BrainComponent>(componentJSON);
			break;
		case ComponentTypes::CHILDREN_COMPONENT:
			componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::CHILDREN_COMPONENT);
			component = std::make_shared<ChildrenComponent>(componentJSON, name, scene);
			break;
		case ComponentTypes::COMPOSITE_COMPONENT:
			componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::COMPOSITE_COMPONENT);
			component = std::make_shared<CompositeComponent>(componentJSON, name, scene);
			break;
		case ComponentTypes::CONTAINER_COMPONENT:
			componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::CONTAINER_COMPONENT);
			component = std::make_shared<ContainerComponent>(componentJSON, name, scene);
			break;
		case ComponentTypes::CHECKPOINT_COMPONENT:
			componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::CHECKPOINT_COMPONENT);
			component = std::make_shared<CheckPointComponent>(componentJSON);
			break;
		case ComponentTypes::HUD_COMPONENT:
			componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::HUD_COMPONENT);
			component = std::make_shared<HudComponent>(componentJSON, scene);
			break;
		case ComponentTypes::INVENTORY_COMPONENT:
			componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::INVENTORY_COMPONENT);
			component = std::make_shared<InventoryComponent>(componentJSON, name, scene);
			break;
		case ComponentTypes::IMGUI_COMPONENT:
			componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::IMGUI_COMPONENT);
			component = std::make_shared<IMGuiComponent>(componentJSON, name, scene);
			break;
		case ComponentTypes::NAVIGATION_COMPONENT:
			componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::NAVIGATION_COMPONENT);
			component = std::make_shared<NavigationComponent>(componentJSON);
			break;
		case ComponentTypes::PARTICLE_COMPONENT:
			componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::PARTICLE_COMPONENT);
			component = std::make_shared<ParticleComponent>(componentJSON);
			break;
		case ComponentTypes::PARTICLE_X_COMPONENT:
			componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::PARTICLE_X_COMPONENT);
			component = std::make_shared<ParticleXComponent>(componentJSON);
			break;
		case ComponentTypes::PHYSICS_COMPONENT:
			//
			//Physics component cannot be overridden right now so pass in the whole gameobject definition is needed
			//
			component = std::make_shared<PhysicsComponent>(definitionJSON, scene, xMapPos, yMapPos, angleAdjust);
			break;
		case ComponentTypes::PLAYER_CONTROL_COMPONENT:
			componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::PLAYER_CONTROL_COMPONENT);
			component = std::make_shared<PlayerControlComponent>(componentJSON);
			break;
		case ComponentTypes::POOL_COMPONENT:
			componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::POOL_COMPONENT);
			component = std::make_shared<PoolComponent>(componentJSON);
			break;
		case ComponentTypes::RENDER_COMPONENT:
			componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::RENDER_COMPONENT);
			component = std::make_shared<RenderComponent>(componentJSON);
			break;
		case ComponentTypes::SOUND_COMPONENT:
			componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::SOUND_COMPONENT);
			component = std::make_shared<SoundComponent>(componentJSON);
			break;
		case ComponentTypes::TEXT_COMPONENT:
			componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::TEXT_COMPONENT);
			component = std::make_shared<TextComponent>(gameObjectTextType, componentJSON);
			break;
		case ComponentTypes::TRANSFORM_COMPONENT:
			componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::TRANSFORM_COMPONENT);
			component = std::make_shared<TransformComponent>(componentJSON, xMapPos, yMapPos, angleAdjust);
			break;
		case ComponentTypes::UICONTROL_COMPONENT:
			componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::UICONTROL_COMPONENT);
			component = std::make_shared<UIControlComponent>(componentJSON);
			break;
		case ComponentTypes::VITALITY_COMPONENT:
			componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::VITALITY_COMPONENT);
			component = std::make_shared<VitalityComponent>(componentJSON);
			break;
		case ComponentTypes::WEAPON_COMPONENT:
			componentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::WEAPON_COMPONENT);
			component = std::make_shared<WeaponComponent>(componentJSON);
			break;
		default:
			assert( true && "This component type did not match any of the existing component types!");

	}

    return component;
}
