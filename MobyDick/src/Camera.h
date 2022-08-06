#ifndef CAMERA_H
#define CAMERA_H

#include <optional>
#include <memory>

#include <SDL2/SDL.h>
#include "glm/glm.hpp"

#include "GameObject.h"
#include "GameConfig.h"
#include "Scene.h"
#include "actions/Action.h"




class Camera
{
public:

	static Camera& instance();
	void setFramePosition(float, float);
	void correctPosition(float& xPosition, float& yPosition);
	void update();
	void init();
	
	const SDL_FRect& frame() {	return m_frame; }
	const float speed() { return m_speed; }
	void setFollowMe(std::shared_ptr<GameObject> gameObject);
	void setFollowMe(std::string gameObjectName);
	std::string getFollowMeName() { return m_followMeName; }
	std::shared_ptr<GameObject> getFollowMeObject() { return m_followMe.lock(); }
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
	SDL_FRect m_frame;

	bool m_atXLimit{};
	bool m_atYLimit{};
	std::weak_ptr<GameObject> m_followMe;
	std::string m_followMeName;
	
	float m_speed{};
	int m_currentState{ CameraState::IDLE };
	glm::vec2 m_currentDestination{};
	std::shared_ptr<Action> m_moveAction{};



};

#endif