#pragma once

#include <memory>
#include <optional>

#include "Action.h"

class ActionFactory {



public:

	virtual std::shared_ptr<Action> create(std::string actionType);


private:




};
