#include "MRStatusManager.h"

MRStatusManager::MRStatusManager() 
	: StatusManager()
{
	initMappings();
}

void MRStatusManager::initMappings()
{

	m_valueMap["LIVES_COUNT"] = StatusItem{6, 6};
	m_valueMap["SCRAP_COUNT"] = StatusItem{ 0, 50 };

}
