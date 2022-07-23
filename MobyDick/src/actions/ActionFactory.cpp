#include "ActionFactory.h"
#include "DefaultMoveAction.h"
#include "DefaultRotateAction.h"
#include "DefaultOnHoverAction.h"
#include "DefaultOnHoverOutAction.h"
#include "PrimitiveMoveAction.h"
#include "DroneMoveAction.h"
#include "LoadNextLevelAction.h"
#include "InteractAction.h"
#include "NoAction.h"

#include <iostream>


std::shared_ptr<Action> ActionFactory::create(std::string actionType)
{
    std::shared_ptr<Action> action;

    if (actionType == "NoAction") {

        action =  std::make_shared<NoAction>();
    }
    else if (actionType == "DefaultMove") {

        action = std::make_shared<DefaultMoveAction>();
    }
    else if (actionType == "DefaultRotate") {

        action = std::make_shared<DefaultRotateAction>();
    }
    else if (actionType == "DefaultHover") {

        action = std::make_shared<DefaultOnHoverAction>();
    }
    else if (actionType == "DefaultHoverOut") {

        action = std::make_shared<DefaultOnHoverOutAction>();
    }
    else if (actionType == "PrimitiveMove") {

        action = std::make_shared<PrimitiveMoveAction>();
    }
    else if (actionType == "DroneMove") {

        action = std::make_shared<DroneMoveAction>();
    }
    else if (actionType == "LoadNextLevel") {

        action = std::make_shared<LoadNextLevelAction>();
    }
    else if (actionType == "Interact") {

        action = std::make_shared<InteractAction>();
    }
    else if (actionType == "COLL_Player_Wall") {

        action = std::make_shared<NoAction>();
    }
    else {
        std::cout << "No Action was Matched" << std::endl;
        action = std::make_shared<NoAction>();
    }

    return action;
}
