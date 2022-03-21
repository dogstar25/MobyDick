#include "PrimitiveMoveAction.h"

#include "../SceneManager.h"


void PrimitiveMoveAction::perform(GameObject* gameObject, Json::Value actionParms)
{

	assert(actionParms.isMember("trajectoryX") && "TrajectoryX value is required");
	assert(actionParms.isMember("trajectoryY") && "TrajectoryY value is required");
	assert(actionParms.isMember("force") && "Force value is required");

	//Get action paramters
	auto trajectoryX = actionParms["trajectoryX"].asFloat();
	auto trajectoryY = actionParms["trajectoryY"].asFloat();
	auto force = actionParms["force"].asFloat();


	float timeFactor{ GameConfig::instance().gameLoopStep() };
	if (SceneManager::instance().gameTimer().timeRemaining().count() > 0) {
		timeFactor = SceneManager::instance().gameTimer().timeRemaining().count();
	}

	auto newPositionX = gameObject->getCenterPosition().x + trajectoryX * force * timeFactor;
	auto newPositionY = gameObject->getCenterPosition().y + trajectoryY * force * timeFactor;


}