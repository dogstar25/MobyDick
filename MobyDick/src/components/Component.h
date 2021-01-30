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
class ParticleXComponent;
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
	virtual void postInit(const std::array <std::vector<std::shared_ptr<GameObject>>, MAX_GAMEOBJECT_LAYERS>&) {};

	void setParent(GameObject* gameObject) { m_parentGameObject = gameObject; }
	GameObject* parent() { return m_parentGameObject; }

	std::string m_gameObjectId{ "" };

protected:
	GameObject* m_parentGameObject{ nullptr };


};

#endif

