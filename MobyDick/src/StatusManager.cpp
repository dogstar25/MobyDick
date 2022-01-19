#include "StatusManager.h"
#include <assert.h>


//StatusManager& StatusManager::instance()
//{
//	static StatusManager singletonInstance;
//	return singletonInstance;
//}

StatusManager::StatusManager()
{
    initMappings();
}

void StatusItem::adjust(float adjustValue)
{
    if (m_value < m_maximumValue) {
        m_value += adjustValue;
    }
}

void StatusItem::reset()
{

    m_value = m_originalValue;

}

float StatusManager::getValue(std::string valueId)
{

    assert(m_valueMap.find(valueId) != m_valueMap.end() && "ValueId Name wasnt found in StatusManager");

    float value = m_valueMap[valueId].value();

    return value;

}

void StatusManager::adjustValue(std::string valueId, float adjustmentValue)
{
    assert(m_valueMap.find(valueId) != m_valueMap.end() && "ValueId Name wasnt found in StatusManager");

    m_valueMap[valueId].adjust(adjustmentValue);

}

void StatusManager::setValue(std::string valueId, float newValue)
{
    assert(m_valueMap.find(valueId) != m_valueMap.end() && "ValueId Name wasnt found in StatusManager");

    m_valueMap[valueId].setValue(newValue);

}

void StatusManager::initMappings()
{
    m_valueMap["LEVEL_COUNT"] = StatusItem{ 0,10 };
    m_valueMap["FPS_VALUE"] = StatusItem{0,60};

}