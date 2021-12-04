#include "StatusManager.h"
#include <assert.h>


//StatusManager& StatusManager::instance()
//{
//	static StatusManager singletonInstance;
//	return singletonInstance;
//}


void StatusManager::initHudMappings()
{
    StatusItem statusItem{};

    statusItem.set(&m_level);
    m_hudValueMap["HUD_LEVEL_VALUE"] = statusItem;

}