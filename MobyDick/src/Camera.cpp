#include "Camera.h"


#include "GameConfig.h"
#include "LevelManager.h"


Camera::~Camera()
{
}

Camera& Camera::instance()
{
	static Camera singletonInstance;
	return singletonInstance;

}

void Camera::update()
{

	if (m_followMe) {

		const auto& followObjectTransformComponent = m_followMe.value()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
		setFramePosition(
			followObjectTransformComponent->position().x -	(m_frame.w / 2),
			followObjectTransformComponent->position().y - (m_frame.h / 2)
		);
	}

}

void Camera::setFramePosition(float x, float y)
{
	float xPos, yPos;

	//If no level is loaded, then frame does not move
	if (LevelManager::instance().m_id.empty() == false)
	{
		//Check far left limit
		if (x < LevelManager::instance().m_levelBounds.x)
		{
			xPos = (float)LevelManager::instance().m_levelBounds.x;
		}
		//Check far right limit
		else if (x > LevelManager::instance().m_levelBounds.w - m_frame.w)
		{
			xPos = LevelManager::instance().m_levelBounds.w - m_frame.w;
		}
		else
		{
			xPos = x;
		}

		//Check far top limit
		if (y < LevelManager::instance().m_levelBounds.y)
		{
			yPos = (float)LevelManager::instance().m_levelBounds.y;
		}
		//Check far bottom limit
		else if (y > LevelManager::instance().m_levelBounds.h - m_frame.h)
		{
			yPos = LevelManager::instance().m_levelBounds.h - m_frame.h;
		}
		else
		{
			yPos = y;
		}

		//set position
		m_frame.x = xPos;
		m_frame.y = yPos;
	}

}


