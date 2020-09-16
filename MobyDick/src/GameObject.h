#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <memory>
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <json/json.h>
#include <map>
#include <array>
#include <unordered_map>
#include <typeindex>
#include <optional>

#include <box2d/box2d.h>

#include "Globals.h"
#include "GameObjectDefinition.h"
#include "components/TransformComponent.h"
#include "components/ActionComponent.h"
#include "components/AnimationComponent.h"
#include "components/AttachmentsComponent.h"
#include "components/ChildrenComponent.h"
#include "components/CompositeComponent.h"
#include "components/InventoryComponent.h"
#include "components/ParticleXComponent.h"
#include "components/ParticleComponent.h"
#include "components/PhysicsComponent.h"
#include "components/PlayerControlComponent.h"
#include "components/RenderComponent.h"
#include "components/TextComponent.h"
#include "components/VitalityComponent.h"
#include "components/WeaponComponent.h"
#include "components/UIControlComponent.h"
#include "components/PoolComponent.h"

//#include "Scene.h"

class Scene;

class GameObject
{
public:
	
	GameObject();
	~GameObject();


	std::string m_id;

	GameObject(std::string gameObjectId, float xMapPos, float yMapPos, float angleAdjust);

	virtual void update();
	virtual void render();

	void setRemoveFromWorld(bool removeFromWorld) { m_removeFromWorld = removeFromWorld; }
	void setPosition(b2Vec2 position, float angle);
	void setPosition(float x, float y);
	void init(bool cameraFollow=false);

	void addComponent(const std::shared_ptr<Component>, ComponentTypes);
	std::shared_ptr<Component> getComponent(ComponentTypes);

	//Accessor Functions
	auto removeFromWorld() { return m_removeFromWorld; }
	std::string id() { return m_id;	}
	int idTag() { return m_idTag; }
	auto const& gameObjectDefinition() { return m_gameObjectDefinition;	}
	//auto& components() { return m_components; }
	auto isPooledAvailable() { return m_isPooledAvailable; }
	void setIsPooledAvailable(int isPooledAvailable);

	void reset();
	void addInventoryItem(GameObject* gameObject);
	void _setDependecyReferences();

	//Components - All public
	std::optional< std::shared_ptr<ActionComponent>> actionComponent;
	std::optional< std::shared_ptr<AnimationComponent>> animationComponent;
	std::optional< std::shared_ptr<AttachmentsComponent>> attachmentsComponent;
	std::optional< std::shared_ptr<ChildrenComponent>> childrenComponent;
	std::optional< std::shared_ptr<CompositeComponent>> compositeComponent;
	std::optional< std::shared_ptr<InventoryComponent>> inventoryComponent;
	std::optional< std::shared_ptr<ParticleComponent>> particleComponent;
	std::optional< std::shared_ptr<ParticleXComponent>> particleXComponent;
	std::optional< std::shared_ptr<PlayerControlComponent>> playerControlComponent;
	std::optional< std::shared_ptr<PhysicsComponent>> physicsComponent;
	std::optional< std::shared_ptr<PoolComponent>> poolComponent;
	std::optional< std::shared_ptr<RenderComponent>> renderComponent;
	std::optional< std::shared_ptr<TextComponent>> textComponent;
	std::optional< std::shared_ptr<TransformComponent>> transformComponent;
	std::optional< std::shared_ptr<UIControlComponent>> uicontrolComponent;
	std::optional< std::shared_ptr<VitalityComponent>> vitalityComponent;
	std::optional< std::shared_ptr<WeaponComponent>> weaponComponent;

private:
	
	int m_idTag{ 0 };
	bool m_removeFromWorld{ false };

	//Special values that need to be outside of components for speed
	bool m_isPooledAvailable{ true };
	

	std::shared_ptr<GameObjectDefinition> m_gameObjectDefinition;

	//Components
	//std::map<std::type_index, std::shared_ptr<Component>>m_components;


	std::array<std::shared_ptr<Component>, MAX_COMPONENTS>m_components_new;

};

#endif
