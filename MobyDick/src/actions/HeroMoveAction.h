#pragma once
#include "Action.h"

class HeroMoveAction : public Action
{

public:
	HeroMoveAction();
	~HeroMoveAction();

	int
		m_direction,
		m_strafe;

	void perform(GameObject* gameObject) override;
	void setMoveParms(int direction, int strafe) override {
		m_direction = direction;
		m_strafe = strafe;
	} 
};