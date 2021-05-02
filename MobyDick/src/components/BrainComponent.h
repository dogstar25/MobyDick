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


protected:

    int m_currentState{ BrainState::PATROL };

    std::vector<IntersectionItem> m_seenObjects;

    int m_sensorLength{};
    int m_sensorOffset{};

    void _updateSensorInput();
        


};

