#pragma once
#include "Component.h"
#include "../GameObject.h"

#include <jsoncpp/include/json/json.h>

#include <memory>

class NavigationComponent : public Component
{

public:
	NavigationComponent() = default;
	NavigationComponent(Json::Value definitionJSON);
	~NavigationComponent();

	void update() override;
	void postInit() override;

	int type() { return m_type; }
	void setType(int type) { m_type = type; }
	std::vector < std::shared_ptr<GameObject>> accessibleNavObjects() { return m_accessibleNavObjects; }

private:

	int m_type{};

	std::vector<std::shared_ptr<GameObject>>m_accessibleNavObjects;

};

