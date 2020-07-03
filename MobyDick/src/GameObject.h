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

	//Accessor Functions
	auto removeFromWorld() { 
		return m_removeFromWorld; 
	}

	auto gameObjectDefinition(){
		return m_gameObjectDefinition;
	}
	auto componentFlags() {
		return m_componentFlags;
	}
	auto& animationComponent() {
		return mAnimationComponent;
	}
	auto& attachmentsComponent() {
		return mAttachmentsComponent;
	}
	auto& childrenComponent() {
		return mChildrenComponent;
	}
	auto& compositeComponent() {
		return mCompositeComponent;
	}
	auto& particleComponent() {
		return mParticleComponent;
	}
	auto& physicsComponent() {
		return mPhysicsComponent;
	}
	auto& renderComponent() {
		return mRenderComponent;
	}
	auto& textComponent() {
		return mTextComponent;
	}
	auto& transformComponent() {
		return mTransformComponent;
	}
	auto& vitalityComponent() {
		return mVitalityComponent;
	}
	auto& weaponComponent() {
		return mWeaponComponent;
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
	AnimationComponent mAnimationComponent;
	AttachmentsComponent mAttachmentsComponent;
	ChildrenComponent mChildrenComponent;
	CompositeComponent mCompositeComponent;
	ParticleComponent mParticleComponent;
	PhysicsComponent mPhysicsComponent;
	RenderComponent mRenderComponent;
	TextComponent mTextComponent;
	TransformComponent mTransformComponent;
	VitalityComponent mVitalityComponent;
	WeaponComponent mWeaponComponent;
	
	

};

#endif
