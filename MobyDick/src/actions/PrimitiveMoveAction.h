#pragma once
#include "MoveAction.h"
#include <SDL2/SDL.h>

class PrimitiveMoveAction : public MoveAction
{

public:
	PrimitiveMoveAction();
	//ActorMoveAction(int direction, int strafe);
	~PrimitiveMoveAction() {};

	void perform(SDL_FRect* gameObjectRect) override;

};
