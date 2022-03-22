#pragma once
#include "Action.h"
class UseAction : public Action
{

public:

    UseAction() = default;

    virtual void perform(GameObject* gameObject) {};

private:

};

