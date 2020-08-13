#include "Camera.h"

#include "GameConfig.h"
#include "Level.h"


Camera::Camera()
{

}

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

		auto& followObjectTransformComponent = m_followMe.value()->getComponent<TransformComponent>();
		setFramePosition(followObjectTransformComponent->position().x -	(m_frame.w / 2),
			followObjectTransformComponent->position().y - (m_frame.h / 2));
	}

}

void Camera::setFramePosition(int x, int y)
{
	int xPos, yPos;

	//If no level is loaded, then frame does not move
	if (Level::instance().m_id.empty() == false)
	{
		//Check far left limit
		if (x < Level::instance().m_levelBounds.x)
		{
			xPos = Level::instance().m_levelBounds.x;
		}
		//Check far right limit
		else if (x > Level::instance().m_levelBounds.w - m_frame.w)
		{
			xPos = Level::instance().m_levelBounds.w - m_frame.w;
		}
		else
		{
			xPos = x;
		}

		//Check far top limit
		if (y < Level::instance().m_levelBounds.y)
		{
			yPos = Level::instance().m_levelBounds.y;
		}
		//Check far bottom limit
		else if (y > Level::instance().m_levelBounds.h - m_frame.h)
		{
			yPos = Level::instance().m_levelBounds.h - m_frame.h;
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

void Camera::init()
{

	m_frame.x = 0;
	m_frame.y = 0;
	m_frame.w = GameConfig::instance().windowWidth();
	m_frame.h = GameConfig::instance().windowHeight();

	setFramePosition(0, 0);


}

