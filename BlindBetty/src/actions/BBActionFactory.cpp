#include "BBActionFactory.h"
#include "actions/ActionFactory.h"
#include "BaseConstants.h"
#include "actions/NoAction.h"


#include <iostream>


std::shared_ptr<Action> BBActionFactory::create(std::string actionType)
{
    std::shared_ptr<Action> action;

    if (actionType == "CustomAction") {

        action = std::make_shared<NoAction>();
    }


    else {
        action = ActionFactory::create(actionType);
    }

    return action;
}
