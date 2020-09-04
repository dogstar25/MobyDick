#pragma once
#include "Action.h"
class UseAction : public Action
{

public:

    UseAction();
    ~UseAction();

    virtual void perform(GameObject* gameObject) {};

private:

};

