#pragma once
#include <json/json.h>
#include "components/Component.h"

#include "components/AnimationComponent.h"
#include "components/ActionComponent.h"
#include "components/AttachmentsComponent.h"
#include "components/BrainComponent.h"
#include "components/ChildrenComponent.h"
#include "components/CompositeComponent.h"
#include "components/ContainerComponent.h"
#include "components/HudComponent.h"
#include "components/InventoryComponent.h"
#include "components/NavigationComponent.h"
#include "components/ParticleComponent.h"
#include "components/ParticleXComponent.h"
#include "components/PoolComponent.h"
#include "components/UIControlComponent.h"
#include "components/PhysicsComponent.h"
#include "components/RenderComponent.h"
#include "components/TextComponent.h"
#include "components/TransformComponent.h"
#include "components/VitalityComponent.h"
#include "components/WeaponComponent.h"
#include "components/PlayerControlComponent.h"
#include "components/IMGuiComponent.h"



class Scene;

class ComponentFactory
{


public:

	ComponentFactory() = default;

	//Create version that has ALL possible inputs
	virtual std::shared_ptr<Component> create(
		Json::Value definitionJSON,
		std::string gameObjectName,
		std::string gameObjectType,
		Scene* scene,
		float xMapPos,
		float yMapPos,
		float angleAdjust,
		const int componentType);

};

