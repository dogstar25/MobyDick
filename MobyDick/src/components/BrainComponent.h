#pragma once
#include "Component.h"
#include "../GameObject.h"
#include "../RayCastCallBack.h"
#include "../LevelManager.h"

#include <json/json.h>

#include <optional>

class BrainComponent : public Component
{

public:
    BrainComponent();
    BrainComponent(Json::Value definitionJSON);
    ~BrainComponent();

    void update();
    void postInit() override;

    int sensorLength() { return m_sensorLength; }
    int sensorOffset() { return m_sensorOffset; }
    int sensorCount() { return m_sensorCount; }

    std::vector<IntersectionItem> seenObjects() { return m_seenObjects; }

protected:

    int m_currentState{ BrainState::PATROL };
    
    int m_sensorLength{};
    int m_sensorOffset{};
    int m_sensorCount{};
    int m_currentSensorIteration{1};

    std::vector<IntersectionItem> m_seenObjects;


    void _updateSensorInput();
    
        


};

