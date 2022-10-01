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
//#include "components/BrainComponent.h"
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

class Scene;

class GameObject
{
public:

	GameObject() {}
	~GameObject();
	
	//Need to define default move constructors because we have an explicit deconstructor defined
	GameObject(GameObject&&) = default;
	GameObject& operator=(GameObject&&) = default;

	GameObject(std::string gameObjectType, float xMapPos, float yMapPos, float angleAdjust, Scene* parentScene, int layer=GameLayer::MAIN, bool cameraFollow=false, std::string name="");

	virtual void update();
	virtual void render();
	virtual void render(SDL_FRect destQuad);

	void setRemoveFromWorld(bool removeFromWorld) { m_removeFromWorld = removeFromWorld; }
	void setPosition(b2Vec2 position, float angle);
	void setPosition(float x, float y);
	void setPosition(SDL_FPoint position);
	void setPosition(PositionAlignment windowPosition, float adjustX=0., float adjustY=0.);
	void setLayer(int layer) { m_layer = layer; }
	void setPhysicsActive(bool active);
	void setParentScene(Scene* parentScene);
	void setAngleInDegrees(float angle);
	void setAngleInRadians(float angle);
	void setColor(SDL_Color color);
	void setWeaponForce(float force);
	void setWeaponColor(SDL_Color color);

	b2Vec2 getSize();
	float getAngle();
	SDL_FPoint getCenterPosition();
	SDL_FPoint getTopLeftPosition();
	bool isPointingAt(SDL_FPoint gameObject);
	bool holdsDependentGameObjects();

	void postInit();
	bool hasTrait(int32_t trait) { return m_traitTags.test(trait); }
	std::bitset<32> traits() {	return m_traitTags;	}
	std::bitset<8> states() { return m_stateTags; }
	void addTrait(int32_t trait) { m_traitTags.set(trait, true); }
	void removeTrait(int32_t trait) { m_traitTags.set(trait, false); }
	void dispatch(SDL_FPoint destination);
	int brainState();

	void disableUpdate();
	void enableUpdate();
	bool updateDisabled() { return m_stateTags.test(StateTag::disabledUpdate); }

	void disablePhysics();
	void enablePhysics();
	bool physicsDisabled() { return m_stateTags.test(StateTag::disabledPhysics); }

	void disableRender();
	void enableRender();
	bool renderDisabled() { return m_stateTags.test(StateTag::disabledRender); }

	void disableCollision();
	void enableCollision();
	bool collisionDisabled() { return m_stateTags.test(StateTag::disabledCollision); }

	//Accessor Functions
	std::string id() { return m_id; }
	std::string name() { return m_name; }
	std::string type() { return m_type; }

	auto removeFromWorld() { return m_removeFromWorld; }
	
	auto const& gameObjectDefinition() { return m_gameObjectDefinition; }
	auto& components() { return m_components; }
	Scene* parentScene() { return m_parentScene; }
	void setCollisionTag(int contactTag);
	int layer() { return m_layer; }

	//void setCollisionTag(int contactTag) { m_contactTag = contactTag; }
	//void resetCollisionTag() { m_contactTag = m_originalCollisionTag; }

	void reset();
	void addInventoryItem(GameObject* gameObject);
	std::vector<GameObject> getTouchingByTrait(const int trait);
	void addTouchingObject(std::shared_ptr<GameObject> touchingObject);
	void setParent(GameObject* parentObject);
	std::optional<GameObject*> parent() { return m_parentObject; }

	const std::unordered_map<std::string, std::weak_ptr<GameObject>>& getTouchingObjects() {
		return m_touchingGameObjects;
	}

	/*template <typename componentType>
	inline std::shared_ptr<componentType> addComponent(std::shared_ptr<componentType> component, ComponentTypes componentTypeIndex)
	{
		m_components[(int)componentTypeIndex] = std::move(component);
		return std::static_pointer_cast<componentType>(m_components.at((int)componentTypeIndex));
	}*/

	inline std::shared_ptr<Component> addComponent(std::shared_ptr<Component> component)
	{
		auto componentType = component->componentType();
		m_components[(int)componentType] = std::move(component);
		return m_components.at((int)componentType);
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

	std::string m_id{};
	std::string m_name{};
	std::string m_type{};
	int m_contactTag{ 0 };

	std::optional<GameObject*> m_parentObject{};
	
	
	bool m_removeFromWorld{ false };
	Scene* m_parentScene{nullptr};
	std::bitset<32> m_traitTags{};
	std::bitset<8> m_stateTags{};
	std::unordered_map<std::string, std::weak_ptr<GameObject>> m_touchingGameObjects{};
	std::shared_ptr<GameObjectDefinition> m_gameObjectDefinition;

	int m_layer;

	//Components
	//std::unordered_map<std::type_index, std::shared_ptr<Component>>m_components;
	std::array<std::shared_ptr<Component>, static_cast<int>(ComponentTypes::MAX_COMPONENT_TYPES)>m_components;

	std::string _buildId(std::string gameObjectType, float xMapPos, float yMapPos);
	std::string _buildName(std::string rootName, std::string gameObjectType, bool isDependent);
	void _updateTouchingObjects();

};

#endif
