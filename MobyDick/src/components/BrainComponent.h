#pragma once
#include "Component.h"
#include "../GameObject.h"
#include "../RayCastCallBack.h"

#include <json/json.h>

class BrainComponent : public Component
{

public:
    BrainComponent();
    BrainComponent(Json::Value definitionJSON);
    ~BrainComponent();

    void update() override;


private:

    RayCastCallBack m_b2RayCastCallback;

};

