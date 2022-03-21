#ifndef Actor_USE_ACTION_H
#define Actor_USE_ACTION_H

#include "Action.h"

class GameObject;

class ActorUseAction : public Action
{
public:

	ActorUseAction() = default;

	void perform(GameObject* gameObject) override;

};

#endif

