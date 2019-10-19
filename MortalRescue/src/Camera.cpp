#include "Camera.h"



void Camera::setPosition(int x, int y)
{
	int xPos, yPos;

	//Check far left limit
	if (x < this->worldBounds->x)
	{
		xPos = this->worldBounds->x;
	}
	//Check far right limit
	else if (x > this->worldBounds->w - this->frame.w)
	{
		xPos = this->worldBounds->w - this->frame.w;
	}
	else
	{
		xPos = x;
	}

	//Check far top limit
	if (y < this->worldBounds->y)
	{
		yPos = this->worldBounds->y;
	}
	//Check far bottom limit
	else if (y > this->worldBounds->h - this->frame.h)
	{
		yPos = this->worldBounds->h - this->frame.h;
	}
	else
	{
		yPos = y;
	}

	//set position
	this->frame.x = xPos;
	this->frame.y = yPos;

}

void Camera::init(SDL_Rect *worldBounds)
{

	this->worldBounds = worldBounds;

}
