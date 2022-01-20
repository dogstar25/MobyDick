#pragma once
#include <string>
#include <optional>

#include "../GameObject.h"

class HudItem
{
public:
	HudItem() = default;

	virtual void update(GameObject* parentGameObject) = 0;
	virtual void render(GameObject* parentGameObject) = 0;
	float labelPadding() { return m_labelPadding; }

protected:
	int m_id{};
	float m_labelPadding{};
	std::optional<std::shared_ptr<GameObject>> m_label{};
	std::shared_ptr<GameObject> m_statusObject{};
	std::string m_statusValueId{};

};

