#pragma once
#include "components\BrainComponent.h"

#include <iostream>
#include <optional>
#include <json/json.h>

#include "Timer.h"

class DroneBrainComponent : public BrainComponent
{

public:
	DroneBrainComponent(Json::Value definitionJSON);

    void update() override;
	void postInit() override;

private:
	std::vector<std::weak_ptr<GameObject>> m_wayPoints;
	SDL_FPoint _getNextPatrolDestination();

	int m_currentWaypointIndex{ 0 };
	std::optional<SDL_FPoint> m_focusPoint{};
	Timer m_engageStateTimer{};
	Timer m_eyeFireDelayTimer{};
	Timer m_patrolStuckTimer{};

	void _doPatrol();
	void _doAlert();
	void _doPursue();
	void _doEngage();

	//float calculateDistance(SDL_FPoint location1, SDL_FPoint location2);
	void _rotateTowards(b2Vec2 targetPoint, b2Vec2 rotationCenter, GameObject* gameObject);
	int _determineState();

	std::optional<SDL_FPoint> _detectPlayer();


};

