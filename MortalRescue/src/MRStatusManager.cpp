#include "MRStatusManager.h"

void MRStatusManager::initHudMappings()
{
	StatusManager::initHudMappings();

	StatusItem statusItem{};
	statusItem.set(&m_lives);
	m_hudValueMap["HUD_LIVES"] = statusItem;

	statusItem.set(&m_scrapCount);
	m_hudValueMap["HUD_SCRAP"] = statusItem;

}
