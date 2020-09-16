#ifndef CAMERA_H
#define CAMERA_H

#include <optional>

#include <SDL2/SDL.h>

#include "GameObject.h"
#include "GameConfig.h"

class Camera
{
public:

	static Camera& instance();
	void setFramePosition(float, float);
	void update();
	
	SDL_FRect& frame() { return m_frame; }
	void setFollowMe(GameObject* gameObject) { m_followMe = gameObject;	}

private:
	Camera() {};
	~Camera();
	SDL_FRect m_frame { 
		0, 0, 
		static_cast<float>(GameConfig::instance().windowWidth()), 
		static_cast<float>(GameConfig::instance().windowHeight()) 
	};

	std::optional<GameObject*> m_followMe;


};

#endif