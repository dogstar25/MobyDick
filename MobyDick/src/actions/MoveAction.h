#pragma once
#include "Action.h"

class MoveAction :  public Action
{

public:

	MoveAction(int direction, int strafe);
	~MoveAction();

	virtual void perform(GameObject* gameObject) {};

	int direction() {
		return m_direction;
	}

	int strafe() {
		return m_strafe;
	}

	void setDirection(int direction) {
		m_direction = direction;
	}

	void setStrafe(int strafe) {
		m_strafe = strafe;
	}

protected:
	int m_direction;
	int m_strafe;

private:

};

