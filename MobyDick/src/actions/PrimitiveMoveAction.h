#pragma once
#include "Action.h"
#include <SDL2/SDL.h>

class PrimitiveMoveAction : public Action
{

public:
	PrimitiveMoveAction() = default;

	void perform(SDL_FRect* gameObjectRect, glm::vec2, float force) override;

};
