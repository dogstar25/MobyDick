#pragma once
#include <SDL2/SDL.h>

class Camera
{
public:

	static Camera& instance();
	void setFramePosition(int x, int y);
	void init();

	SDL_Rect frame() {
		return m_frame;
	}
	SDL_Rect cameraBounds() {
		return m_cameraBounds;
	}

private:
	Camera();
	~Camera();
	SDL_Rect m_frame;
	SDL_Rect m_cameraBounds;



};

