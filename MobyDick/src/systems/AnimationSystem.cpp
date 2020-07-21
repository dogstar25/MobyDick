#include "AnimationSystem.h"
#include <chrono>

#include "../game.h"
#include "../components/RenderComponent.h"
#include "../components/AnimationComponent.h"


void AnimationSystem::init()
{
	/*
	Build the component signature for this system
	*/
	Signature signature;
	signature.set(Game::instance().gameCoordinator().GetComponentType<AnimationComponent>());
	signature.set(Game::instance().gameCoordinator().GetComponentType<RenderComponent>());

	Game::instance().gameCoordinator().SetSystemSignature<AnimationSystem>(signature);
}

void AnimationSystem::update()
{

	/*
	Could probably call the box2d step here
	FixMe
	*/
	for (auto& entity : mEntities)
	{
		auto& animationComponent = Game::instance().gameCoordinator().GetComponent<AnimationComponent>(entity);
		auto& renderComponent = Game::instance().gameCoordinator().GetComponent<RenderComponent>(entity);

		//Get the current animation item and copy its texture to the render component
		auto& animation = animationComponent.m_animations[animationComponent.m_currentAnimationState];
		renderComponent.m_texture->sdlTexture = animation->m_texture;

		//Update the animation texture source rectangle and then copy it and the current animation texture to the 
		//RenderComponent for later rendering

		//check the clock and see if enough time as gone by to animate
		std::chrono::steady_clock::time_point now_time = std::chrono::steady_clock::now();
		std::chrono::duration<double> time_diff = now_time - animation->m_timeSnapshot;

		if (time_diff.count() >= animation->m_speed)
		{
			animation->m_timeSnapshot = now_time;

			//Increment animation frame counter and reset if it exceeds last one
			animation->m_currentAnimFrame += 1;
			if (animation->m_currentAnimFrame >
				animation->m_frameCount - 1) {

				animation->m_currentAnimFrame = 0;
			}

			//build the rectangle that points to the current animation frame
			std::shared_ptr<SDL_Rect> rect = std::make_shared<SDL_Rect>();

			rect->x = animation->m_animationFramePositions[animation->m_currentAnimFrame].x;
			rect->y = animation->m_animationFramePositions[animation->m_currentAnimFrame].y;

			rect->w = animation->m_frameSize.x;
			rect->h = animation->m_frameSize.y;

			//animation->m_currentTextureAnimationSrcRect = rect;

			//Copy the source rect to the render component
			renderComponent.m_currentTextureAnimationSrcRect = rect;

		}
	}

}
