#include "MRActionFactory.h"
#include "actions/ActionFactory.h"
#include "BaseConstants.h"
#include "actions/NoAction.h"
#include "DroneEyeRotateAction.h"
#include "ToggleButtonInteraction.h"
#include "OnetimeButtonInteraction.h"
#include "TurretFireAction.h"
#include "SurvivorInteractionAction.h"
#include "ActorBoostAction.h"
#include "ActorSprintAction.h"
#include "HealthStationInteraction.h"
#include "ActorMoveAction.h"
#include "ActorRotateAction.h"
#include "ActorUseAction.h"
#include "PistolFireAction.h"
#include "PistolSecondaryFireAction.h"


#include <iostream>


std::shared_ptr<Action> MRActionFactory::create(std::string actionType)
{
    std::shared_ptr<Action> action;

    if (actionType == "DroneEyeRotateAction") {

        action = std::make_shared<DroneEyeRotateAction>();
    }
	else if (actionType == "ToggleButtonInteraction") {

		action = std::make_shared<ToggleButtonInteraction>();
	}
	else if (actionType == "OnetimeButtonInteraction") {

		action = std::make_shared<OnetimeButtonInteraction>();
	}
	else if (actionType == "TurretFire") {

		action = std::make_shared<TurretFireAction>();
	}
	else if (actionType == "SurvivorInteraction") {

		action = std::make_shared<SurvivorInteractionAction>();
	}
	else if (actionType == "ActorBoost") {

		action = std::make_shared<ActorBoostAction>();
	}
	else if (actionType == "HealthStationInteraction") {

		action = std::make_shared<HealthStationInteraction>();
	}
	else if (actionType == "ActorSprintAction") {

		action = std::make_shared<ActorSprintAction>();
	}
	else if (actionType == "ActorUse") {

		action = std::make_shared<ActorUseAction>();
	}
	else if (actionType == "ActorMove") {

		action = std::make_shared<ActorMoveAction>();
	}
	else if (actionType == "ActorRotate") {

		action = std::make_shared<ActorRotateAction>();
	}
	else if (actionType == "PistolFire") {

		action = std::make_shared<PistolFireAction>();
	}
	else if (actionType == "PistolSecondaryFire") {

		action = std::make_shared<PistolSecondaryFireAction>();
	}


	

    else {
        action = ActionFactory::create(actionType);
    }

    return action;
}
