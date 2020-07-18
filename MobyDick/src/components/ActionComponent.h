#ifndef ACTION_COMPONENT_H
#define ACTION_COMPONENT_H

#include "Component.h"

#include <map>
#include <memory>
#include <map>

#include "../actions/Action.h"
#include "../Globals.h"


struct ActionComponent
{
	std::map<int, std::shared_ptr<Action>>m_actionMap;

};


#endif
