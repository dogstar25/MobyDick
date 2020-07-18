#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <memory>
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <json/json.h>
#include <map>
#include <unordered_map>
#include <typeindex>

#include <Box2D/Box2D.h>

#include "GameObjectDefinition.h"
#include "components/TransformComponent.h"
#include "components/ActionComponent.h"
#include "components/AnimationComponent.h"
#include "components/AttachmentsComponent.h"
#include "components/ChildrenComponent.h"
#include "components/CompositeComponent.h"
#include "components/ParticleComponent.h"
#include "components/PhysicsComponent.h"
#include "components/PlayerControlComponent.h"
#include "components/RenderComponent.h"
#include "components/TextComponent.h"
#include "components/VitalityComponent.h"
#include "components/WeaponComponent.h"

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
	
	GameObject();
	~GameObject();


	std::string m_id;

	GameObject(std::string gameObjectId, float xMapPos, float yMapPos, float angleAdjust);

	virtual void update();
	virtual void render();

	void setRemoveFromWorld(bool removeFromWorld) { 
		m_removeFromWorld = removeFromWorld; 
	}

	void setPosition(b2Vec2 position, float angle);
	//bool hasComponent(int componentId);
	//std::shared_ptr<Component> getRefComponent(int componentId);

	/*
	Template function to add component to map
	*/
	template <typename componentType>
	inline void addComponent(std::shared_ptr<componentType> component)
	{
		m_components[std::type_index(typeid (*component))] = component;
	}

	template <typename componentType>
	inline std::shared_ptr<componentType> getComponent()
	{
	
		std::type_index index (typeid(componentType));
		if (m_components.count(std::type_index(typeid(componentType))) != 0)
		{
			return std::static_pointer_cast<componentType>(m_components[index]);
		}
		else
		{
			return nullptr;
		}
	}

	template <typename componentType> bool hasComponent() {
		return(m_components.count(std::type_index(typeid(componentType))) != 0);
	}

	void removeComponent(int componentId);

	//Accessor Functions
	auto removeFromWorld() { 
		return m_removeFromWorld; 
	}
	std::string id() {
		return m_id;
	}
	auto const& gameObjectDefinition(){
		return m_gameObjectDefinition;
	}
	auto& components() {
		return m_components;
	}
	void reset();
	void addInventoryItem(std::shared_ptr<GameObject>inventoryObject);
	void _setDependecyReferences();

private:

	
	int
		m_mouseState;
	bool
		m_removeFromWorld;

	std::shared_ptr<GameObjectDefinition> m_gameObjectDefinition;

	//Components
	//std::array<std::shared_ptr<Component>, 32>m_components;
	std::map<std::type_index, std::shared_ptr<Component>>m_components;
	std::bitset<32> m_componentFlags;

};

#endif
