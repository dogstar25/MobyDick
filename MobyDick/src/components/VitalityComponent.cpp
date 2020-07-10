#include "VitalityComponent.h"

#include "../GameObjectManager.h"
#include "../GameObject.h"

VitalityComponent::VitalityComponent()
{

}


VitalityComponent::VitalityComponent(Json::Value definitionJSON, GameObject* gameObject) :
	Component(gameObject)
{

	Json::Value vitalityComponentJSON = definitionJSON["vitalityComponent"];

	m_speed = vitalityComponentJSON["speed"].asFloat();


}

VitalityComponent::~VitalityComponent()
{

}

void VitalityComponent::update()
{

}




