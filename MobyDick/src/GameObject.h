#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <memory>
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <json/json.h>

#include <Box2D/Box2D.h>

#include "GameObjectDefinition.h"
#include "components/TransformComponent.h"
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

	GameObject(std::string gameObjectId, float xMapPos, float yMapPos, float angleAdjust);

	virtual void update();
	virtual void render();

	void setRemoveFromWorld(bool removeFromWorld) { 
		m_removeFromWorld = removeFromWorld; 
	}

	void setPosition(b2Vec2 position, float angle);


	//Accessor Functions
	auto removeFromWorld() { 
		return m_removeFromWorld; 
	}

	auto const& gameObjectDefinition(){
		return m_gameObjectDefinition;
	}
	auto& animationComponent() {
		return m_AnimationComponent;
	}
	auto& attachmentsComponent() {
		return m_AttachmentsComponent;
	}
	auto& childrenComponent() {
		return m_ChildrenComponent;
	}
	auto& compositeComponent() {
		return m_CompositeComponent;
	}
	auto& particleComponent() {
		return m_ParticleComponent;
	}
	auto& physicsComponent() {
		return m_PhysicsComponent;
	}
	auto& playerControlComponent() {
		return m_PlayerControlComponent;
	}
	auto& renderComponent() {
		return m_RenderComponent;
	}
	auto& textComponent() {
		return m_TextComponent;
	}
	auto& transformComponent() {
		return m_TransformComponent;
	}
	auto& vitalityComponent() {
		return m_VitalityComponent;
	}
	auto& weaponComponent() {
		return m_WeaponComponent;
	}

private:

	void _setDependecyReferences();
	int
		m_mouseState;
	bool
		m_removeFromWorld;
	std::shared_ptr<GameObjectDefinition> m_gameObjectDefinition;
	std::string m_id;


	//Components
	std::shared_ptr<AnimationComponent> m_AnimationComponent;
	std::shared_ptr<AttachmentsComponent> m_AttachmentsComponent;
	std::shared_ptr<ChildrenComponent> m_ChildrenComponent;
	std::shared_ptr<CompositeComponent> m_CompositeComponent;
	std::shared_ptr<ParticleComponent> m_ParticleComponent;
	std::shared_ptr<PhysicsComponent> m_PhysicsComponent;
	std::shared_ptr<RenderComponent> m_RenderComponent;
	std::shared_ptr<TextComponent> m_TextComponent;
	std::shared_ptr<TransformComponent> m_TransformComponent;
	std::shared_ptr<VitalityComponent> m_VitalityComponent;
	std::shared_ptr<WeaponComponent> m_WeaponComponent;
	std::shared_ptr<PlayerControlComponent> m_PlayerControlComponent;
	
	

};

#endif
