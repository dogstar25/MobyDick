#ifndef VITALITY_COMPONENT_H
#define VITALITY_COMPONENT_H

#include <chrono>

#include "Component.h"

class GameObject;

struct VitalityComponent
{
	float speed;
	float strength;
	float health;

	VitalityComponent(Json::Value definitionJSON);
};

#endif

