#pragma once
#include "Action.h"

class InteractAction : public Action
{


public:

    InteractAction() = default;

    void perform(GameObject* gameObject, Json::Value actionParms);


};

