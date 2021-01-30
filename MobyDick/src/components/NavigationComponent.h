#pragma once
#include "Component.h"
#include "../GameObject.h"

#include <json/json.h>

class NavigationComponent : public Component
{

public:
	NavigationComponent() = default;
	NavigationComponent(Json::Value definitionJSON);
	~NavigationComponent();

	void update() override;

	bool isWaypoint() { return m_isWaypoint; }
	void setIsWaypoint(bool isWaypoint) { m_isWaypoint = isWaypoint; }

private:

	bool m_isWaypoint{};

	std::vector<GameObject*>m_accessibleNavObjects;

};

