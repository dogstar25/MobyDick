#pragma once
#include "Action.h"
class ActorWarpAction : public Action
{


public:
	ActorWarpAction() = default;

	void perform(GameObject* interactingObject, GameObject* interactionObject, b2Vec2 destination) override;
};

