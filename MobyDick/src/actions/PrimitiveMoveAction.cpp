#include "PrimitiveMoveAction.h"

#include "../SceneManager.h"
#include "../GameConfig.h"


PrimitiveMoveAction::PrimitiveMoveAction() :
	MoveAction(0,0)
{

}
void PrimitiveMoveAction::perform(SDL_FRect* gameObjectRect)
{


	float timeFactor{ GameConfig::instance().gameLoopStep() };
	if (SceneManager::instance().gameTimer().timeRemaining().count() > 0) {
		timeFactor = SceneManager::instance().gameTimer().timeRemaining().count();
	}

	//float angle{};
	//float xVelocity = cos(angle) * (m_force * 50);
	//float yVelocity = sin(angle) * (m_force * 50);

	gameObjectRect->x += m_trajectory.x * m_force * timeFactor;
	gameObjectRect->y += m_trajectory.y * m_force * timeFactor;


}