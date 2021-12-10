#include "MRStatusManager.h"

void MRStatusManager::initHudMappings()
{
	StatusManager::initHudMappings();

	m_hudValueMap["HUD_LIVES_VALUE"] = StatusItem{&m_lives, 6};


	m_hudValueMap["HUD_SCRAP_VALUE"] = StatusItem{ &m_scrapCount, 50 };
	
	m_hudValueMap["HUD_BAR"] = StatusItem{ &m_scrapCount, 75 };


}
