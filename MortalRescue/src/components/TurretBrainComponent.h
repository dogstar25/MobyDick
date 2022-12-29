#pragma once
#include "components\BrainComponent.h"

#include <iostream>
#include <optional>
#include <json/json.h>

#include "Timer.h"

class TurretBrainComponent : public BrainComponent
{

public:
	TurretBrainComponent(Json::Value definitionJSON);

	void update() override;

private:

	int _determineState();
	void _doEngage();
	void _doIdle();
	void _doDeploy();
	void _doUnDeploy();
	bool _rotateTowards(b2Vec2 targetPoint);
	std::optional<SDL_FPoint> _detectPlayer();

	Timer m_fireDelayTimer{};

};
