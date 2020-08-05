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
	virtual void update();
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
	void setParent(GameObject* gameObject) {

		m_parentGameObject = gameObject;
	}
	GameObject* parent() {

		return m_parentGameObject;
	}

	std::string m_gameObjectId;

protected:
	bool m_active;
	GameObject* m_parentGameObject;


};

#endif

