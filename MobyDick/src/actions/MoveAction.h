#pragma once
#include "Action.h"

#include <box2d/box2d.h>

class MoveAction :  public Action
{

public:

	MoveAction(int direction, int strafe);
	MoveAction(b2Vec2 trajectory);
	~MoveAction();

	void setMoveParms(int direction, int strafe);
	void setMoveParms(b2Vec2 trajectory);

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

	void setTrajectory(b2Vec2 trajectory) {
		m_trajectory = trajectory;
	}

protected:
	int m_direction;
	int m_strafe;
	b2Vec2 m_trajectory;

private:

};

