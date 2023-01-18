#pragma once

#include <memory>
#include <optional>

#include "actions/Action.h"
#include "actions/ActionFactory.h"

class BBActionFactory : public ActionFactory{



public:

	std::shared_ptr<Action> create(std::string actionType) override;


private:




};
