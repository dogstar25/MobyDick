#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <bitset>
#include <json/json.h>

#include "../Util.h"
#include "../BaseConstants.h"

class GameObject;
class AnimationComponent;
class ActionComponent;
class AttachmentsComponent;
class ChildrenComponent;
class CompositeComponent;
class ParticleXComponent;
class PhysicsComponent;
class RenderComponent;
class TextComponent;
class TransformComponent;
class VitalityComponent;
class WeaponComponent;
class PlayerControlComponent;
class IMGuiComponent;
class Component
{

public:

	Component();
	~Component();
	virtual void update() = 0;
	virtual void postInit() {};
	void disable() { disabled = true; }
	void enable() { disabled = false; }
	bool isDisabled() { return disabled; }

	virtual void setParent(GameObject* gameObject) { m_parentGameObject = gameObject; }
	GameObject* parent() { return m_parentGameObject; }

protected:
	GameObject* m_parentGameObject{ nullptr };
	std::string m_gameObjectId{};
	bool disabled{ false };


};

#endif

