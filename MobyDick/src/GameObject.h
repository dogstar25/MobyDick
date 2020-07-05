#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <memory>
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <json/json.h>
#include <bitset>

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
	//GameObject(std::string gameObjectId, float xMapPos, float yMapPos, float angleAdjust) :
	//	mTransformComponent(gameObjectId) {}
	~GameObject();

	GameObject(std::string gameObjectId, float xMapPos, float yMapPos, float angleAdjust);

	virtual void update();
	virtual void render();

	void setRemoveFromWorld(bool removeFromWorld) { 
		m_removeFromWorld = removeFromWorld; 
	}

	auto setComponentFlag(size_t flag) {
		m_componentFlags.set(flag, true);
	}

	bool hasComponentFlag(size_t flag) {
		return m_componentFlags.test(flag);
	}
	void performMovementSequence(float velocity, int direction, int strafe);
	void performRotationSequence(float angularVelocity);
	void setAnimationState(int animationState);
	void playSound(int soundId);

	//Accessor Functions
	auto removeFromWorld() { 
		return m_removeFromWorld; 
	}

	auto gameObjectDefinition(){
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

	int
		m_mouseState;
	bool
		m_removeFromWorld;
	std::shared_ptr<GameObjectDefinition> m_gameObjectDefinition;
	std::string m_id;
	std::bitset<32>
		m_componentFlags;


	void init();

	//Components
	AnimationComponent m_AnimationComponent;
	AttachmentsComponent m_AttachmentsComponent;
	ChildrenComponent m_ChildrenComponent;
	CompositeComponent m_CompositeComponent;
	ParticleComponent m_ParticleComponent;
	PhysicsComponent m_PhysicsComponent;
	RenderComponent m_RenderComponent;
	TextComponent m_TextComponent;
	TransformComponent m_TransformComponent;
	VitalityComponent m_VitalityComponent;
	WeaponComponent m_WeaponComponent;
	PlayerControlComponent m_PlayerControlComponent;
	
	

};

#endif
