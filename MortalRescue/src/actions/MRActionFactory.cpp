#include "MRActionFactory.h"
#include "actions/ActionFactory.h"
#include "BaseConstants.h"
#include "actions/NoAction.h"
#include "DroneEyeRotateAction.h"
#include "MyAction.h"

#include <iostream>


std::shared_ptr<Action> MRActionFactory::create(std::string actionType)
{
    std::shared_ptr<Action> action;

    if (actionType == "MyAction") {

        action =  std::make_shared<MyAction>();
    }
    else if (actionType == "DroneEyeRotateAction") {

        action = std::make_shared<DroneEyeRotateAction>();
    }

    else {
        action = ActionFactory::create(actionType);
    }

    return action;
}
