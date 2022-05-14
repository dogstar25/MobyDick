#pragma once
#include "Action.h"

class InteractAction : public Action
{


public:

    InteractAction() = default;

    void perform(GameObject* gameObject, SDL_Scancode keyScanCode) override;


};

