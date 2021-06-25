#pragma once

#include <iostream>
#include <optional>

#include <json/json.h>

#include "components\BrainComponent.h"

class DroneBrainComponent : public BrainComponent
{

public:
    DroneBrainComponent(Json::Value definitionJSON) : BrainComponent(definitionJSON) {};

    void update() override;
	void postInit() override;

private:
	std::optional<std::weak_ptr<GameObject>> m_targetDestination{};
	std::optional<std::weak_ptr<GameObject>> m_interimDestination{};
	std::vector<std::optional<std::weak_ptr<GameObject>>> m_wayPoints;
	std::vector<std::optional<std::weak_ptr<GameObject>>> m_navPoints;
	std::vector<std::weak_ptr<GameObject>> m_tempVisitedNavPoints;
	std::optional<float> m_targetAngle{};
	std::weak_ptr<GameObject> getClosestNavPoint(SDL_FPoint position, int NavType);
	std::shared_ptr<GameObject> getNextTargetDestination();
	std::shared_ptr<GameObject> getNextinterimDestination();

	int m_currentWaypointIndex{ 0 };
	SDL_FPoint m_targetLocation{0,0};

	std::chrono::steady_clock::time_point engageStateTimeSnapshot{ std::chrono::steady_clock::now() };
	//std::chrono::duration<float, std::milli> lifetime;


	void _doPatrol();
	void _doAlert();
	void _doPursue();
	void _doEngage();

	float calculateDistance(SDL_FPoint location1, SDL_FPoint location2);
	bool existsInAlreadyVistedNavList(std::weak_ptr<GameObject> navPoint);
	void _rotateTowards(b2Vec2 targetPoint);
	void _applyAvoidanceMovement();
	int _determineState();

	std::optional<SDL_FPoint> _detectPlayer();

	void navigate();

};

