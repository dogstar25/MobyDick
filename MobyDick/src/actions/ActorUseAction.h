#ifndef Actor_USE_ACTION_H
#define Actor_USE_ACTION_H

#include "Action.h"
#include "UseAction.h"

class GameObject;

class ActorUseAction : public UseAction
{
public:

	ActorUseAction() = default;

	void perform(GameObject* gameObject) override;

};

#endif

