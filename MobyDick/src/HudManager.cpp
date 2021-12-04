#include "HudManager.h"

HudManager::~HudManager()
{

}

HudManager& HudManager::instance()
{

	static HudManager singletonInstance;
	return singletonInstance;

}

void HudManager::updateHud(const int id, int statusCount)
{
}

void HudManager::updateHud(const int id, std::string text)
{
}

void HudManager::update()
{
}

void HudManager::render()
{
}
