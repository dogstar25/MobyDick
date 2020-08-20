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

	void setForce(float force) {
		m_force = force;
	}

	//Accessor functions
	float speed() {
		return m_speed;
	}

private:
	float
		m_speed,
		m_force,
		m_durability,
		m_health;



};

#endif

