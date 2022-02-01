#pragma once
#include "Action.h"

#include <box2d/box2d.h>
#include <SDL2/SDL.h>
#include "glm/glm.hpp"

class MoveAction :  public Action
{

public:

	MoveAction(int direction, int strafe);
	MoveAction(b2Vec2 trajectory);
	~MoveAction();

	void setMoveParms(int direction, int strafe);
	void setMoveParms(b2Vec2 trajectory);
	void setMoveParms(glm::vec2 trajectory, float force);

	virtual void perform(GameObject* gameObject) {};
	virtual void perform(SDL_FRect* gameObjectRect) {};

	int direction() { return m_direction;}
	int strafe() {	return m_strafe;}
	float force() {	return m_force;	}

	void setDirection(int direction) {
		m_direction = direction;
	}

	void setStrafe(int strafe) {
		m_strafe = strafe;
	}

	void setTrajectory(b2Vec2 trajectory) {
		m_trajectory = trajectory;
	}

	void setForce(float force) {
		m_force = force;
	}

protected:
	int m_direction{};
	int m_strafe{};
	b2Vec2 m_trajectory{};
	float m_force{};

private:

};

