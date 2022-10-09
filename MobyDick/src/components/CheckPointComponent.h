#pragma once
#include "Component.h"
#include "../GameObject.h"

#include <json/json.h>

#include <memory>

class CheckPointComponent : public Component
{

public:
	CheckPointComponent() = default;
	CheckPointComponent(Json::Value componentJSON);
	~CheckPointComponent();

	void update() override;
	void postInit() override;

	SDL_FPoint playerSpawnLocation() { return m_playerReSpawnLocation; }
	int sequence() { return m_sequence; }
	bool contactRequired() { return m_contactRequired; }
	bool hasMetCriteria() { return m_hasMetCriteria; }

	void setContactMade(bool contactMade) { m_contactMade = true; }
	bool contactMade() { return m_contactMade; }

private:

	SDL_FPoint m_playerReSpawnLocation{};
	bool m_contactRequired{};
	bool m_contactMade{};
	int m_sequence{};
	std::optional<std::shared_ptr<Trigger>> m_trigger{};
	bool m_hasMetCriteria {};

};

