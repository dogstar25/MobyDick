#ifndef VITALITY_COMPONENT_H
#define VITALITY_COMPONENT_H

#include <chrono>

#include "Component.h"

class GameObject;

class VitalityComponent :  public Component
{
public:
	VitalityComponent();
	VitalityComponent(Json::Value definitionJSON);
	~VitalityComponent();

	void update() override;

	//Accessor functions
	float speed() {
		return m_speed;
	}

private:
	float
		m_speed,
		m_strength,
		m_health;



};

#endif

