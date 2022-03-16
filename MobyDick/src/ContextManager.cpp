#include "ContextManager.h"
#include "SoundManager.h"

#include <assert.h>


//StatusManager& StatusManager::instance()
//{
//	static StatusManager singletonInstance;
//	return singletonInstance;
//}

ContextManager::ContextManager()
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

float ContextManager::getValue(std::string valueId)
{

    assert(m_statusValueMap.find(valueId) != m_statusValueMap.end() && "ValueId Name wasnt found in StatusValueMap");

    float value = m_statusValueMap[valueId].value();

    return value;

}

void ContextManager::adjustValue(std::string valueId, float adjustmentValue)
{
    assert(m_statusValueMap.find(valueId) != m_statusValueMap.end() && "ValueId Name wasnt found in StatusValueMap");

    m_statusValueMap[valueId].adjust(adjustmentValue);

}

void ContextManager::setValue(std::string valueId, float newValue)
{
    assert(m_statusValueMap.find(valueId) != m_statusValueMap.end() && "ValueId Name wasnt found in StatusValueMap");

    m_statusValueMap[valueId].setValue(newValue);

}

void ContextManager::initMappings()
{
    m_statusValueMap["LEVEL_COUNT"] = StatusItem{ 0,10 };
    m_statusValueMap["FPS_VALUE"] = StatusItem{0,60};

}

void ContextManager::setMouseSensitivity(int mouseSensitivity)
{

    m_userSettings.mouseSensitivity = mouseSensitivity;

}

void ContextManager::setSoundVolume(int volume)
{

    m_userSettings.soundLevel = volume;
	//SoundManager::instance().setVolume(volume);

}

int ContextManager::getMouseSensitivityForGui()
{
    return  m_userSettings.mouseSensitivity;

}

int ContextManager::getSoundVolumeForGui()
{
	return  m_userSettings.soundLevel;

}

float ContextManager::getMouseSensitivity()
{
	return  m_userSettings.mouseSensitivity * .01;

}
int ContextManager::getSoundVolume()
{
	return  (int)(m_userSettings.soundLevel * 1.28);

}
