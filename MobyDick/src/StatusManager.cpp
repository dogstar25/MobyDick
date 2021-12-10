#include "StatusManager.h"
#include <assert.h>


//StatusManager& StatusManager::instance()
//{
//	static StatusManager singletonInstance;
//	return singletonInstance;
//}


void StatusItem::set(float* value, float maximumValue)
{

    m_value = value;
    m_originalValue = *value;
    m_maximumValue = maximumValue;

}
void StatusItem::adjust(float adjustValue)
{
    if (*m_value < m_maximumValue) {
        *m_value += adjustValue;
    }
}

void StatusItem::reset()
{

    *m_value = m_originalValue;

}

void StatusManager::initHudMappings()
{
    StatusItem statusItem{};

    statusItem.set(&m_level, 10);
    m_hudValueMap["HUD_LEVEL_VALUE"] = statusItem;

}