#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <memory>
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <json/json.h>
#include <map>

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

class GameObject
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
	inline void addComponent(int componentId, std::shared_ptr<componentType> component)
	{
		m_components.emplace(componentId, component);
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

	void _setDependecyReferences();

private:

	
	int
		m_mouseState;
	bool
		m_removeFromWorld;

	std::shared_ptr<GameObjectDefinition> m_gameObjectDefinition;

	//Components
	std::map<int, std::shared_ptr<Component>>m_components;

};

#endif
