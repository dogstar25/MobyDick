#pragma once
#include "Component.h"
#include "../GameObject.h"
#include "../BrainAABBCallback.h"
#include "../LevelManager.h"

#include <json/json.h>
#include "glm/glm.hpp"

#include <optional>

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

    std::vector<BrainAABBFoundObject> seenObjects() { return m_seenObjects; }
    std::vector<BrainAABBFoundObject> detectedObjects() { return m_detectedObjects; }

protected:

    int m_currentState{ BrainState::PATROL };
    std::bitset<32> m_detectObjectTraits{};
    
    std::vector<GameObject*> m_navPoints;
    std::vector<GameObject*> m_tempVisitedNavPoints;

    std::optional<GameObject*> m_targetDestination{};
    std::optional<GameObject*> m_interimDestination{};
    

    //int m_sensorLength{};
    //int m_sensorOffset{};
    //int m_sensorCount{};
    int m_currentSensorIteration{1};

    int m_sightSensorSize{};

    std::vector<BrainAABBFoundObject> m_seenObjects;
    std::vector<BrainAABBFoundObject> m_detectedObjects;

    GameObject* getClosestNavPoint(SDL_FPoint targetPosition, int navType);
    GameObject* getClosestSeenNavPoint(SDL_FPoint targetPosition, int navType);
    std::optional<GameObject*> getNextinterimDestination();
    bool navigate();
    void executeMove();
    void stopMovement();

private:
    void _updateSensorInput();
    bool _existsInAlreadyVistedNavList(GameObject* navPoint);
    bool _hasLineOfSight(BrainAABBFoundObject& detectedObject);
    bool _doDispatch();
    void _doIdle();


};

