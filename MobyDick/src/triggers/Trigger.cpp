#include "Trigger.h"


bool Trigger::hasMetCriteria(Scene* scene) 
{


	if (m_criteriaCheckTimer.has_value() && m_criteriaCheckTimer.value().hasMetTargetDuration()) {
		return true;
	}

	return false;

}