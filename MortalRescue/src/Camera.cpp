#include "Camera.h"
#include "GameConfig.h"


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

void Camera::setFrameSize(int width, int height)
{

	m_frame.w = width;
	m_frame.h = height;

}


void Camera::setFramePosition(int x, int y)
{
	int xPos, yPos;

	//Check far left limit
	if (x < m_cameraBounds.x)
	{
		xPos = m_cameraBounds.x;
	}
	//Check far right limit
	else if (x > m_cameraBounds.w - m_frame.w)
	{
		xPos = m_cameraBounds.w - m_frame.w;
	}
	else
	{
		xPos = x;
	}

	//Check far top limit
	if (y < m_cameraBounds.y)
	{
		yPos = m_cameraBounds.y;
	}
	//Check far bottom limit
	else if (y > m_cameraBounds.h - m_frame.h)
	{
		yPos = m_cameraBounds.h - m_frame.h;
	}
	else
	{
		yPos = y;
	}

	//set position
	m_frame.x = xPos;
	m_frame.y = yPos;

}

void Camera::init()
{

	m_frame.x = GameConfig::instance().defaultCameraFrame().x;
	m_frame.y = GameConfig::instance().defaultCameraFrame().y;
	m_frame.w = GameConfig::instance().defaultCameraFrame().w;
	m_frame.h = GameConfig::instance().defaultCameraFrame().h;


}

void Camera::setCameraBounds(SDL_Rect bounds)
{

	m_cameraBounds = bounds;

}
