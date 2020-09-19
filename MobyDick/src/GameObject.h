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

#pragma warning(push,0)
#include <box2d/box2d.h>
#pragma warning(pop)

#include "GameObjectDefinition.h"
#include "components/TransformComponent.h"
#include "components/ActionComponent.h"
#include "components/AnimationComponent.h"
#include "components/AttachmentsComponent.h"
#include "components/ChildrenComponent.h"
#include "components/CompositeComponent.h"
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

	//Accessor Functions
	auto removeFromWorld() { return m_removeFromWorld; }
	std::string id() { return m_id;	}
	int idTag() { return m_idTag; }
	auto const& gameObjectDefinition() { return m_gameObjectDefinition;	}
	auto& components() { return m_components; }
	auto isPooledAvailable() { return m_isPooledAvailable; }
	void setIsPooledAvailable(int isPooledAvailable);

	void reset();
	void addInventoryItem(GameObject* gameObject);
	void _setDependecyReferences();

	template <typename componentType>
	inline void addComponent(std::shared_ptr<componentType> component, ComponentTypes componentTypeIndex)
	{
		m_components[(int)componentTypeIndex] = component;
	}

	template <typename componentType>
	inline std::shared_ptr<componentType> getComponent(ComponentTypes componentTypeIndex)
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

	bool hasComponent(ComponentTypes componentTypeIndex) {

		if (!m_components[(int)componentTypeIndex]) {
			return false;
		}
		else {
			return true;
		}
	}

private:
	
	int m_idTag{ 0 };
	bool m_removeFromWorld{ false };

	//Special values that need to be outside of components for speed
	bool m_isPooledAvailable{ true };
	

	std::shared_ptr<GameObjectDefinition> m_gameObjectDefinition;

	//Components
	//std::unordered_map<std::type_index, std::shared_ptr<Component>>m_components;
	std::array<std::shared_ptr<Component>, (int)ComponentTypes::COUNT+1>m_components;

};

#endif
