#ifndef CAMERA_H
#define CAMERA_H

#include <optional>

#include <SDL2/SDL.h>

#include "GameObject.h"

class Camera
{
public:

	static Camera& instance();
	void setFramePosition(int x, int y);
	void update();
	void init();

	SDL_FRect frame() {
		return m_frame;
	}
	SDL_FRect cameraBounds() {
		return m_cameraBounds;
	}
	void setFollowMe(GameObject* gameObject) {
		m_followMe = gameObject;
	}

private:
	Camera();
	~Camera();
	SDL_FRect m_frame;
	SDL_FRect m_cameraBounds;

	std::optional<GameObject*> m_followMe;


};

#endif