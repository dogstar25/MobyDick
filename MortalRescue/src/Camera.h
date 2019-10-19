#pragma once
#include <SDL.h>

class Camera
{
public:

	SDL_Rect frame;
	SDL_Rect* worldBounds;


	void setPosition(int x, int y);
	void init(SDL_Rect* worldBounds);


};

