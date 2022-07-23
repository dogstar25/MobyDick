#ifndef Actor_USE_ACTION_H
#define Actor_USE_ACTION_H

#include "actions/Action.h"
#include "actions/UseAction.h"

class GameObject;

class ActorUseAction : public UseAction
{
public:

	ActorUseAction() = default;

	void perform(GameObject* gameObject, int usageParm) override;

};

#endif

