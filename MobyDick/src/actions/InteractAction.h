#pragma once
#include "Action.h"
class InteractAction : public Action
{


public:

    InteractAction() {};
    InteractAction(std::tuple<std::string, int, float> params) :
        m_params(params) {}

    void setParams(std::tuple<std::string, int, float> params) {
        m_params = params;
    }

    void perform(GameObject* gameObject){};

protected:
    std::tuple<std::string, int, float> m_params;

};

