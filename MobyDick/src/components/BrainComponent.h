#pragma once
#include "Component.h"
#include "../GameObject.h"
#include "../BrainAABBCallback.h"
#include "../LevelManager.h"

#include <json/json.h>

#include <optional>

class BrainComponent : public Component
{

public:
    BrainComponent();
    BrainComponent(Json::Value componentJSON);
    ~BrainComponent();

    void update() override;
    void postInit() override;

    int sensorLength() { return m_sensorLength; }
    int sensorOffset() { return m_sensorOffset; }
    int sensorCount() { return m_sensorCount; }
    int sightSensorSize() { return m_sightSensorSize; }

    std::vector<BrainAABBFoundObject> seenObjects() { return m_seenObjects; }
    std::vector<BrainAABBFoundObject> detectedObjects() { return m_detectedObjects; }

protected:

    int m_currentState{ BrainState::PATROL };
    
    int m_sensorLength{};
    int m_sensorOffset{};
    int m_sensorCount{};
    int m_currentSensorIteration{1};

    int m_sightSensorSize{};

    std::vector<BrainAABBFoundObject> m_seenObjects;
    std::vector<BrainAABBFoundObject> m_detectedObjects;

    void _updateSensorInput();
    bool _hasLineOfSight( BrainAABBFoundObject& detectedObject);


};

