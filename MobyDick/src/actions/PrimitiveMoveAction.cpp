#include "PrimitiveMoveAction.h"

#include "../SceneManager.h"


void PrimitiveMoveAction::perform(SDL_FRect* gameObjectRect, glm::vec2 trajectory, float force)
{


	float timeFactor{ GameConfig::instance().gameLoopStep() };
	if (SceneManager::instance().gameTimer().timeRemaining().count() > 0) {
		timeFactor = SceneManager::instance().gameTimer().timeRemaining().count();
	}

	//float angle{};
	//float xVelocity = cos(angle) * (m_force * 50);
	//float yVelocity = sin(angle) * (m_force * 50);

	gameObjectRect->x += trajectory.x * force * timeFactor;
	gameObjectRect->y += trajectory.y * force * timeFactor;


}