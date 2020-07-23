#ifndef VITALITY_COMPONENT_H
#define VITALITY_COMPONENT_H

#include <chrono>

#include "Component.h"

struct VitalityComponent
{
	float speed=0;
	float strength=0;
	float health=0;

	VitalityComponent(){}
	VitalityComponent(Json::Value definitionJSON);
};

#endif

