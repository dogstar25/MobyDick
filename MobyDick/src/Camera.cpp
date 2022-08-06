#include "Camera.h"


#include "game.h"

extern std::unique_ptr<Game> game;

Camera::Camera()
{
	//Get the move action that does not require a physics component
	m_moveAction = std::dynamic_pointer_cast<Action>(game->actionFactory()->create("PrimitiveMove"));

	m_frame = {0, 0, static_cast<float>(game->gameScreenResolution().x), static_cast<float>(game->gameScreenResolution().y) };

}

Camera& Camera::instance()
{
	static Camera singletonInstance;
	return singletonInstance;

}

void Camera::setFollowMe(std::shared_ptr<GameObject> gameObject)
{
	m_followMe = gameObject;
	m_followMeName = gameObject->name();
	m_currentState = CameraState::FOLLOW;
}

void Camera::setFollowMe(std::string gameObjectName)
{
	
	m_followMe.reset();
	m_followMeName = gameObjectName;

}

bool Camera::dispatch(glm::vec2 destination)
{

	m_followMe.reset();
	m_currentState = CameraState::DISPATCH;

	//Check the limits of the camera frame
	correctPosition(destination.x, destination.y);
	m_currentDestination = destination;

	return true;
}

void Camera::update()
{

	if (m_currentState == CameraState::FOLLOW) {

		assert(m_followMe.expired() == false && "No Target for the Camera to follow");

		const auto& followObjectTransformComponent = m_followMe.lock()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
		setFramePosition(
			followObjectTransformComponent->position().x -	(m_frame.w / 2),
			followObjectTransformComponent->position().y - (m_frame.h / 2)
		);
	}
	else if (m_currentState == CameraState::DISPATCH) {

		glm::vec2 currentLocation{ m_frame.x, m_frame.y };
		//if we are at the destination then stop and set the state to idle
		if (util::calculateDistance(getCenterPosition(), m_currentDestination) < DESTINATION_DISTANCE_TOLERANCE) {

			m_currentState = CameraState::IDLE;

		}
		else {

			glm::vec2 trajectory{};
			float speed{20};

			trajectory.x = m_currentDestination.x - getCenterPosition().x;
			trajectory.y = m_currentDestination.y - getCenterPosition().y;
			trajectory = glm::normalize(trajectory);

			m_moveAction->perform(&m_frame, trajectory, speed * PRACTICLE_MOVE_SPEED_ADJ);

		}

	}

	//Correct the camera position if it ran beyound the edges of the level
	correctPosition(m_frame.x, m_frame.y);

}

void Camera::init()
{


	setCurrentState(CameraState::IDLE);
	m_followMe.reset();
	m_followMeName={};

}

void Camera::setFramePosition(float x, float y)
{
	//set position - round to nearest 1 decimal place
	//m_frame.x = std::ceil(x * 10.0) / 10.0;
	//m_frame.y = std::ceil(y * 10.0) / 10.0;

	//Trunc to fix micro screen jittering on camera moving
	m_frame.x = trunc(x);
	m_frame.y = trunc(y);

}

glm::vec2 Camera::getCenterPosition()
{
	glm::vec2 center{};

	center.x = m_frame.x + static_cast<float>(game->gameScreenResolution().x / 2);
	center.y = m_frame.y + static_cast<float>(game->gameScreenResolution().y / 2);

	return center;
}

void Camera::correctPosition(float& xPosition, float& yPosition)
{
	
	//Check far left limit
	if (xPosition < LevelManager::instance().m_levelBounds.x)
	{
		xPosition = (float)LevelManager::instance().m_levelBounds.x;
	}
	//Check far right limit
	else if (xPosition > LevelManager::instance().m_levelBounds.w - m_frame.w)
	{
		xPosition = LevelManager::instance().m_levelBounds.w - m_frame.w;
	}

	//Check far top limit
	if (yPosition < LevelManager::instance().m_levelBounds.y)
	{
		yPosition = (float)LevelManager::instance().m_levelBounds.y;
	}
	//Check far bottom limit
	else if (yPosition > LevelManager::instance().m_levelBounds.h - m_frame.h)
	{
		yPosition = LevelManager::instance().m_levelBounds.h - m_frame.h;
	}

}
