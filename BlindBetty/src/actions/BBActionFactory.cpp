#include "BBActionFactory.h"
#include "actions/ActionFactory.h"
#include "BaseConstants.h"
#include "actions/NoAction.h"
#include "ActorMoveAction.h"
#include "DoorInteractionAction.h"


#include <iostream>


std::shared_ptr<Action> BBActionFactory::create(std::string actionType)
{
    std::shared_ptr<Action> action;

    if (actionType == "CustomAction") {

        action = std::make_shared<NoAction>();
    }
    else if (actionType == "DoorInteractionAction") {

        action = std::make_shared<DoorInteractionAction>();
    }
    else if (actionType == "ActorMove") {

        action = std::make_shared<ActorMoveAction>();
    }


    else {
        action = ActionFactory::create(actionType);
    }

    return action;
}
