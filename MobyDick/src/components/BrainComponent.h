#pragma once
#include "Component.h"
#include "../LevelManager.h"
#include "../BrainAABBCallback.h"

#include <json/json.h>
#include "glm/glm.hpp"
//#include "../GameObject.h"

#include <optional>

class GameObject;

class BrainComponent : public Component
{

public:
    BrainComponent();
    BrainComponent(Json::Value componentJSON);
    ~BrainComponent();

    void update() override;
    void postInit() override;
    void setTargetDestination(SDL_FPoint destination);
    void dispatch(SDL_FPoint destination);
    void dispatch();
    int state() { return m_currentState; }

    //int sensorLength() { return m_sensorLength; }
    //int sensorOffset() { return m_sensorOffset; }
    //int sensorCount() { return m_sensorCount; }
    int sightSensorSize() { return m_sightSensorSize; }

    std::vector<std::weak_ptr<GameObject>> seenObjects() { return m_seenObjects; }
    std::vector<std::weak_ptr<GameObject>> detectedObjects() { return m_detectedObjects; }

protected:

    int m_currentState{ BrainState::PATROL };
    std::bitset<32> m_detectObjectTraits{};
    
    std::vector<std::shared_ptr<GameObject>> m_navPoints;
    std::vector<std::shared_ptr<GameObject>> m_tempVisitedNavPoints;

    std::optional<std::shared_ptr<GameObject>> m_targetDestination{};
    std::optional<std::shared_ptr<GameObject>> m_interimDestination{};
    

    //int m_sensorLength{};
    //int m_sensorOffset{};
    //int m_sensorCount{};
    int m_currentSensorIteration{1};

    int m_sightSensorSize{};

    std::vector<std::weak_ptr<GameObject>> m_seenObjects;
    std::vector<std::weak_ptr<GameObject>> m_detectedObjects;

    std::shared_ptr<GameObject> getClosestNavPoint(SDL_FPoint targetPosition, int navType);
    std::shared_ptr<GameObject> getClosestSeenNavPoint(SDL_FPoint targetPosition, int navType);
    std::optional<std::shared_ptr<GameObject>> getNextinterimDestination();
    bool navigate();
    void executeMove();
    void stopMovement();

private:
    void _updateSensorInput();
    bool _existsInAlreadyVistedNavList(GameObject* navPoint);
    bool _hasLineOfSight(BrainAABBFoundObject& detectedObject);
    bool _doDispatch();
    void _doIdle();

    Timer m_updateSensorInputTimer{0.25, true};


};

