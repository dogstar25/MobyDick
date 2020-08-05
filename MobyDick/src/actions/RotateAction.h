#pragma once
#include "Action.h"
class RotateAction :  public Action
{
public:

    RotateAction(float angularVelocity);
    ~RotateAction();

    virtual void perform(GameObject* gameObject) {};

protected:
    float m_angularVelocity;

};

