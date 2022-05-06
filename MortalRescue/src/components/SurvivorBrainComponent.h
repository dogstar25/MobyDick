#pragma once
#pragma once
#include "components\BrainComponent.h"

#include <iostream>
#include <optional>
#include <json/json.h>

#include "Timer.h"

class SurvivorBrainComponent : public BrainComponent
{

	

public:
	SurvivorBrainComponent(Json::Value definitionJSON);


	void update() override;
	void followMe(GameObject* gameObjectToFollow);
	void stay();

	const int SURVIVOR_FOLLOW_TOLERANCE = 90;

private:

	int _determineState();
	void _doFollow();
	void _doLost();
	void _doIdle();
	bool _rotateTowards(b2Vec2 targetPoint);
	std::optional<SDL_FPoint> _detectPlayer();
	bool _detectFollowedObject();
	void _stayBehindFollowedObject();
	bool _isTouchingBarrier();

	GameObject* m_gameObjectToFollow{};
	Timer m_lostTimer;




};
