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
	//std::optional<GameObject*> m_targetDestination{};
	//std::optional<GameObject*> m_interimDestination{};
	std::vector<std::shared_ptr<GameObject>> m_wayPoints;
	//std::vector<GameObject*> m_navPoints;
	//std::vector<GameObject*> m_tempVisitedNavPoints;
	//GameObject* getClosestNavPoint(SDL_FPoint targetPosition, int navType);
	std::shared_ptr<GameObject> getNextPatrolDestination();

	int m_currentWaypointIndex{ 0 };
	SDL_FPoint m_targetLocation{0,0};
	Timer m_engageStateTimer{};
	Timer m_eyeFireDelayTimer{};
	Timer m_patrolStuckTimer{};
	std::optional<SDL_FPoint> m_previousLocation{};


	void _doPatrol();
	void _doAlert();
	void _doPursue();
	void _doEngage();

	//float calculateDistance(SDL_FPoint location1, SDL_FPoint location2);
	void _rotateTowards(b2Vec2 targetPoint, b2Vec2 rotationCenter, GameObject* gameObject);
	void _applyAvoidanceMovement();
	int _determineState();

	std::optional<SDL_FPoint> _detectPlayer();

	//void navigatePatrol();
	void _navigateEngage();

};

