#pragma once
#include "Action.h"
class RotateAction :  public Action
{
public:

    RotateAction(float angularVelocity);
    ~RotateAction();

    virtual void perform(GameObject* gameObject) {};

    float angularVelocity() {
        return m_angularVelocity;
    }

    void setAngularVelocity(float angularVelocity) {
        m_angularVelocity = angularVelocity;
    }

protected:
    float m_angularVelocity;

};

