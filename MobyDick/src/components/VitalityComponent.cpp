#include "VitalityComponent.h"

#include "../GameObjectManager.h"

VitalityComponent::VitalityComponent(Json::Value definitionJSON)
{

	Json::Value vitalityComponentJSON = definitionJSON["vitalityComponent"];

	speed = vitalityComponentJSON["speed"].asFloat();


}





