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

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

#include "GameObjectDefinition.h"
//#include "components/TransformComponent.h"
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
#include "components/UIControlComponent.h"
#include "components/PoolComponent.h"

//#include "Scene.h"

class Scene;

class GameObject : public sf::Drawable
{
public:
	
	GameObject();
	~GameObject();


	std::string m_id;

	GameObject(std::string gameObjectId, float xMapPos, float yMapPos, float angleAdjust);

	virtual void update();
	virtual void render();

	void setRemoveFromWorld(bool removeFromWorld) { m_removeFromWorld = removeFromWorld; }
	void setPosition(sf::Vector2f position, float angle);
	void setPosition(float x, float y);
	void init(bool cameraFollow=false);

	//Accessor Functions
	auto removeFromWorld() { return m_removeFromWorld; }
	std::string id() { return m_id;	}
	int idTag() { return m_idTag; }
	auto const& gameObjectDefinition() { return m_gameObjectDefinition;	}
	auto& components() { return m_components; }
	auto isPooledAvailable() { return m_isPooledAvailable; }
	void setIsPooledAvailable(int isPooledAvailable);
	bool absolutePositioning() {
		return m_absolutePositioning;
	}
	sf::Vector2f size() {
		return m_size;
	}

	sf::Vector2f setSize(float width, float height) {
		m_size = {width, height};
	}

	void reset();
	void addInventoryItem(GameObject* gameObject);
	void _setDependecyReferences();

	template <typename componentType>
	inline void addComponent(std::shared_ptr<componentType> component)
	{
		m_components[std::type_index(typeid (*component))] = component;
	}

	template <typename componentType>
	inline std::shared_ptr<componentType> getComponent()
	{

		std::type_index index(typeid(componentType));
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

private:
	
	int m_idTag{ 0 };
	bool m_removeFromWorld{ false };

	//Transform Variables
	sf::Vector2f	m_size;
	sf::Vector2f	m_originalPosition;
	bool			m_absolutePositioning;
	bool			m_centeredPositioning;

	//Special values that need to be outside of components for speed
	bool m_isPooledAvailable{ true };

	std::shared_ptr<GameObjectDefinition> m_gameObjectDefinition;

	//Components
	std::map<std::type_index, std::shared_ptr<Component>>m_components;

};

#endif
