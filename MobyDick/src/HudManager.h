#pragma once
#include <string>
#include <memory>
#include <array>

#include "hud/HudItem.h"


class HudManager
{

public:
	static HudManager& instance();
	void updateHud(const int id, int statusCount);
	void updateHud(const int id, std::string text);
	void update();
	void render();

private:
	HudManager();
	~HudManager();

	//std::array<std::shared_ptr<HudItem>, HudItemTypes::MAX_HUD_ITEM_TYPES> m_hudItems;

};

