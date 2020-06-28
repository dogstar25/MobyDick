#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>

class Camera
{
public:

	static Camera& instance();
	void setFramePosition(int x, int y);
	void init();

	SDL_FRect frame() {
		return m_frame;
	}
	SDL_FRect cameraBounds() {
		return m_cameraBounds;
	}

private:
	Camera();
	~Camera();
	SDL_FRect m_frame;
	SDL_FRect m_cameraBounds;



};

#endif