#include "MRActionFactory.h"
#include "actions/ActionFactory.h"
#include "BaseConstants.h"
#include "actions/NoAction.h"
#include "DroneEyeRotateAction.h"
#include "ButtonInteraction.h"
#include "TurretFireAction.h"

#include <iostream>


std::shared_ptr<Action> MRActionFactory::create(std::string actionType)
{
    std::shared_ptr<Action> action;

    if (actionType == "DroneEyeRotateAction") {

        action = std::make_shared<DroneEyeRotateAction>();
    }
	else if (actionType == "ButtonInteraction") {

		action = std::make_shared<ButtonInteraction>();
	}
	else if (actionType == "TurretFire") {

		action = std::make_shared<TurretFireAction>();
	}
    else {
        action = ActionFactory::create(actionType);
    }

    return action;
}
