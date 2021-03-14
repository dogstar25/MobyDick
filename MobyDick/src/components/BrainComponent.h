#pragma once
#include "Component.h"
#include "../GameObject.h"
#include "../RayCastCallBack.h"
#include "../Level.h"

#include <json/json.h>

#include <optional>

class BrainComponent : public Component
{

public:
    BrainComponent();
    BrainComponent(Json::Value definitionJSON);
    ~BrainComponent();

    void update() override;
    void postInit() override;

private:

    int m_currentState{ BrainState::PATROL };

    //Navigation Related
    std::optional<std::weak_ptr<GameObject>> m_targetDestination{};
    std::optional<std::weak_ptr<GameObject>> m_interimDestination{};
    std::vector<std::optional<std::weak_ptr<GameObject>>> m_wayPoints;
    std::vector<std::optional<std::weak_ptr<GameObject>>> m_navPoints;
    std::vector<std::weak_ptr<GameObject>> m_tempVisitedNavPoints;
    std::optional<float> m_targetAngle{};

    int m_currentWaypointIndex{0};

    void _updateSightInput();
    void _updateSensorInput();
        
    void _doPatrol();
    void _doAlert();
    void _doPursue();
    void _doEngage();

    std::weak_ptr<GameObject> getClosestNavPoint(SDL_FPoint position, int NavType);
    float calculateDistance(SDL_FPoint location1, SDL_FPoint location2);
    std::shared_ptr<GameObject> getNextTargetDestination();
    std::shared_ptr<GameObject> getNextinterimDestination();
    bool existsInAlreadyVistedNavList(std::weak_ptr<GameObject> navPoint);
    void _rotateTowards(b2Vec2 targetPoint);
    
    void navigate();


};

