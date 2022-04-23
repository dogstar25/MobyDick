#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <memory>
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <iostream>
#include <json/json.h>
#include <map>
#include <unordered_map>
#include <typeindex>
#include <array>
#include <bitset>

#include <box2d/box2d.h>

#include "BaseConstants.h"

#include "GameObjectDefinition.h"
#include "components/TransformComponent.h"
#include "components/ActionComponent.h"
#include "components/AnimationComponent.h"
#include "components/AttachmentsComponent.h"
#include "components/BrainComponent.h"
#include "components/ChildrenComponent.h"
#include "components/CompositeComponent.h"
#include "components/NavigationComponent.h"
#include "components/ParticleXComponent.h"
#include "components/HudComponent.h"
#include "components/InventoryComponent.h"
#include "components/IMGuiComponent.h"
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

	GameObject() {}
	~GameObject();
	
	//Need to define default move constructors because we have an explicit deconstructor defined
	GameObject(GameObject&&) = default;
	GameObject& operator=(GameObject&&) = default;

	std::string m_id;
	

	//int m_gameObjectType{ GameObjectType::SPRITE };

	GameObject(std::string gameObjectId, float xMapPos, float yMapPos, float angleAdjust, Scene* parentScene, bool cameraFollow=false, std::string name="");

	virtual void update();
	virtual void render();
	virtual void render(SDL_FRect destQuad);

	void setRemoveFromWorld(bool removeFromWorld) { m_removeFromWorld = removeFromWorld; }
	void setPosition(b2Vec2 position, float angle);
	void setPosition(float x, float y);
	void setPosition(SDL_FPoint position);
	void setPosition(PositionAlignment windowPosition);

	bool isPointingAt(SDL_FPoint gameObject);
	
	b2Vec2 getSize();
	SDL_FPoint getCenterPosition();
	SDL_FPoint getTopLeftPosition();
	
	void postInit();
	void setPhysicsActive(bool active);
	void setParentScene( Scene* parentScene);
	void setAngleInDegrees(float angle);
	void setAngleInRadians(float angle);
	bool hasTrait(int32_t trait) { return m_traitTags.test(trait); }
	std::bitset<32> traits() {	return m_traitTags;	}
	void addTrait(int32_t trait) { m_traitTags.set(trait, true); }
	void removeTrait(int32_t trait) { m_traitTags.set(trait, false); }
	void dispatch(SDL_FPoint destination);
	int brainState();

	bool disabled() { return m_disabled; }
	void disable(bool disablePhysicsBody);
	void enable();

	//Accessor Functions
	std::string name() { return m_name; }
	auto removeFromWorld() { return m_removeFromWorld; }
	std::string id() { return m_id; }
	int collisionTag() { return m_collisionTag; }
	auto const& gameObjectDefinition() { return m_gameObjectDefinition; }
	auto& components() { return m_components; }
	Scene* parentScene() { return m_parentScene; }
	void setCollisionTag(int collisionTag) { m_collisionTag = collisionTag; }
	void resetCollisionTag() { m_collisionTag = m_originalCollisionTag; }

	void reset();
	void addInventoryItem(GameObject* gameObject);
	std::vector<GameObject> getTouchingByTrait(const int);
	void addTouchingObject(GameObject* touchingObject) {

		m_touchingGameObjects[touchingObject->name()] = touchingObject;

	}

	const std::unordered_map<std::string, GameObject*>& getTouchingObjects() {
		return m_touchingGameObjects;
	}

	/*template <typename componentType>
	inline std::shared_ptr<componentType> addComponent(std::shared_ptr<componentType> component, ComponentTypes componentTypeIndex)
	{
		m_components[(int)componentTypeIndex] = std::move(component);
		return std::static_pointer_cast<componentType>(m_components.at((int)componentTypeIndex));
	}*/

	inline std::shared_ptr<Component> addComponent(std::shared_ptr<Component> component, const int componentTypeIndex)
	{
		m_components[(int)componentTypeIndex] = std::move(component);
		return m_components.at((int)componentTypeIndex);
	}


	template <typename componentType>
	inline std::shared_ptr<componentType> getComponent(const int componentTypeIndex)
	{

		if (hasComponent(componentTypeIndex))
		{
			return std::static_pointer_cast<componentType>(m_components.at((int)componentTypeIndex));
		}
		else
		{
			return nullptr;
		}
	}

	bool hasComponent(const int componentTypeIndex) {

		if (!m_components[(int)componentTypeIndex]) {
			return false;
		}
		else {
			return true;
		}
	}

	bool operator==(GameObject &gameObject2) {

		std::cout << "Match/n";

		std::shared_ptr<TransformComponent> transformThis = this->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
		std::shared_ptr<TransformComponent> transform2 = gameObject2.getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

		if (this->m_id == gameObject2.m_id) {

			if (transformThis->position() == transform2->position()) {
				return true;
			}

		}
		return false;

	}


private:
	
	std::string m_name;
	int m_collisionTag{ 0 };
	int m_originalCollisionTag{ 0 };
	bool m_removeFromWorld{ false };
	Scene* m_parentScene{nullptr};
	std::bitset<32> m_traitTags{};
	bool m_disabled{};
	std::unordered_map<std::string, GameObject*> m_touchingGameObjects{};


	std::shared_ptr<GameObjectDefinition> m_gameObjectDefinition;

	//Components
	//std::unordered_map<std::type_index, std::shared_ptr<Component>>m_components;
	std::array<std::shared_ptr<Component>, static_cast<int>(ComponentTypes::MAX_COMPONENT_TYPES)>m_components;

	std::string _buildName(std::string id, float xMapPos, float yMapPos, Scene* parentScene);
	void _updateTouchingObjects();

	void _init(std::string gameObjectId, Json::Value definitionJSON, float xMapPos, float yMapPos, float angleAdjust, Scene* parentScene, int layer, bool cameraFollow = false);
};

#endif
