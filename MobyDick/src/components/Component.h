#pragma once

#include <memory>
#include <bitset>
#include <json/json.h>

#include "../Util.h"
#include "../BaseConstants.h"

class GameObject;

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
	int componentType() { return m_componentType; }

protected:
	GameObject* m_parentGameObject{ nullptr };
	std::string m_gameObjectId{};
	int m_componentType{};
	bool disabled{ false };


};

