#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <bitset>
#include <json/json.h>

#include "../Globals.h"

class GameObject;
class AnimationComponent;
class ActionComponent;
class AttachmentsComponent;
class ChildrenComponent;
class CompositeComponent;
class ParticleComponent;
class PhysicsComponent;
class RenderComponent;
class TextComponent;
class TransformComponent;
class VitalityComponent;
class WeaponComponent;
class PlayerControlComponent;

class Component
{

public:

	Component();
	~Component();
	virtual void update(std::shared_ptr<GameObject> gameObject);
	void setDependencyReferences(std::map<int, std::shared_ptr<Component>>components);
	//bool hasRefComponent(int componentId);
	//std::shared_ptr<Component> getRefComponent(int componentId);
	//void setComponentFlags(std::bitset<32> gameObjectComponentFlags) {
	//	m_componentFlags = gameObjectComponentFlags;
	//}
	void setActive(bool active) {
		m_active = active;
	}
	bool isActive() {
		return m_active;
	}

	std::string m_gameObjectId;

protected:
	//Component References
	//std::map<int, std::shared_ptr<Component>>m_refcomponents;

private:
	//std::bitset<32> m_componentFlags;
	bool m_active;


	//std::shared_ptr<AnimationComponent> m_refAnimationComponent;
	//std::shared_ptr<ActionComponent> m_refActionComponent;
	//std::shared_ptr<AttachmentsComponent> m_refAttachmentsComponent;
	//std::shared_ptr<ChildrenComponent> m_refChildrenComponent;
	//std::shared_ptr<CompositeComponent> m_refCompositeComponent;
	//std::shared_ptr<ParticleComponent> m_refParticleComponent;
	//std::shared_ptr<PhysicsComponent> m_refPhysicsComponent;
	//std::shared_ptr<RenderComponent> m_refRenderComponent;
	//std::shared_ptr<TextComponent> m_refTextComponent;
	//std::shared_ptr<TransformComponent> m_refTransformComponent;
	//std::shared_ptr<VitalityComponent> m_refVitalityComponent;
	//std::shared_ptr<WeaponComponent> m_refWeaponComponent;
	//std::shared_ptr<PlayerControlComponent> m_refPlayerControlComponent;

};

#endif

