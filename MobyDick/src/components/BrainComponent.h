#pragma once
#include "Component.h"
#include "../GameObject.h"
#include "../RayCastCallBack.h"
#include "../Level.h"

#include <json/json.h>

class BrainComponent : public Component
{

public:
    BrainComponent();
    BrainComponent(Json::Value definitionJSON);
    ~BrainComponent();

    void update() override;


private:

    int m_currentState{ BrainState::PATROL };
    RayCastCallBack m_b2RayCastCallback;
    //std::vector<Waypoint> m_waypoints;

    void _updateSightInput();
    void _updateSensorInput();
        
    void _doPatrol();
    void _doAlert();
    void _doPursue();
    void _doEngage();


};

