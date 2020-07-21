#pragma once
#include <SDL2/SDL.h>

#include "../ecs/System.hpp"

class TransformComponent;
class RenderComponent;
class AnimationComponent;

class RenderSystem : public System
{
public:
	void init();

	void update();

	SDL_FRect getRenderDestRect(RenderComponent& renderComponent, TransformComponent& transformComponent);
	SDL_Rect* getRenderTextureRect(RenderComponent& renderComponent);
	SDL_Texture* getRenderTexture(RenderComponent& renderComponent);

};

