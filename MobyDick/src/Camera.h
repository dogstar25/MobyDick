#ifndef CAMERA_H
#define CAMERA_H

#include <optional>
#include <memory>

#include <SDL2/SDL.h>
#include "glm/glm.hpp"

#include "GameObject.h"
#include "GameConfig.h"

#include "actions/Action.h"



class Camera
{
public:

	static Camera& instance();
	void setFramePosition(float, float);
	void correctPosition(float& xPosition, float& yPosition);
	void update();
	
	const SDL_FRect& frame() {	return m_frame; }
	const float speed() { return m_speed; }
	void setFollowMe(GameObject* gameObject);
	std::optional<GameObject*> getFollowMeObject() { return m_followMe; }
	bool dispatch(glm::vec2 destination);
	void setSpeed(float speed);
	void setCurrentState(int state) { m_currentState = state; }
	int state() { return m_currentState; }
	glm::vec2 getCenterPosition();
	bool isAtXLimit() { return m_atXLimit; }
	bool isAtYLimit() { return m_atYLimit; }

private:
	Camera();
	~Camera() = default;
	SDL_FRect m_frame { 
		0, 0, 
		static_cast<float>(GameConfig::instance().windowWidth()), 
		static_cast<float>(GameConfig::instance().windowHeight()) 
	};

	bool m_atXLimit{};
	bool m_atYLimit{};
	std::optional<GameObject*> m_followMe;
	float m_speed{};
	int m_currentState{ CameraState::IDLE };
	glm::vec2 m_currentDestination{};
	std::shared_ptr<MoveAction> m_moveAction{};

	void _move(glm::vec2 trajectory, float force);


};

#endif