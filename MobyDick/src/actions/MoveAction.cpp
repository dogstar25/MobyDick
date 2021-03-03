#include "MoveAction.h"


MoveAction::MoveAction(int direction, int strafe) : m_direction(direction), m_strafe(strafe) 
{

}

MoveAction::MoveAction(b2Vec2 trajectory) : m_trajectory(trajectory)
{
	m_direction = 0;
	m_strafe = 0;
}

MoveAction::~MoveAction()
{

}





