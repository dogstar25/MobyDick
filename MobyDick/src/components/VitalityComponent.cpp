#include "VitalityComponent.h"

#include "../GameObjectManager.h"

VitalityComponent::VitalityComponent()
{

}


VitalityComponent::VitalityComponent(Json::Value itrJSON)
{

	Json::Value vitalityComponentJSON = itrJSON["vitalityComponent"];

	m_speed = vitalityComponentJSON["speed"].asFloat();


}

VitalityComponent::~VitalityComponent()
{

}

void VitalityComponent::update()
{

}




