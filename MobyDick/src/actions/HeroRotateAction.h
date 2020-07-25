#pragma once
#include "Action.h"

class HeroRotateAction : public Action
{

public:
	HeroRotateAction();
	~HeroRotateAction();

	void perform(GameObject* gameObject) override;

	void setRotateParms(float angularVelocity) {
		m_angularVelocity = angularVelocity;
	};

private:
	float m_angularVelocity;

};

