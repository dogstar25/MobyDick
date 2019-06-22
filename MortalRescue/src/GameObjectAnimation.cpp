#include "GameObjectAnimation.h"
#include "GameObject.h"
#include "game.h"

void GameObjectAnimation::animate(GameObject* gameObject)
{
	//check the clock and see if enough time as gone by
	steady_clock::time_point now_time = steady_clock::now();
	std::chrono::duration<double> time_diff = now_time - time_snapshot;
	//std::cout << "timediff is " << time_diff.count() << "\n";

	if (time_diff.count() >= this->speed)
	{
		time_snapshot = now_time;

		//Increment animation frame counter and reset if it exceeds last one
		this->currentAnimFrame += 1;
		if (this->currentAnimFrame >
			this->frameCount - 1) {

			this->currentAnimFrame = 0;
		}

		//build the rectangle that points to the current animation frame
		SDL_Rect rect;

		rect.x = this->animationFramePositions[currentAnimFrame].x;
		rect.y = this->animationFramePositions[currentAnimFrame].y;

		//todo:find way to not have to pass in gameObject for this
		rect.w = gameObject->definition->xSize * game->config.scaleFactor;
		rect.h = gameObject->definition->ySize * game->config.scaleFactor;

		this->currentTextureAnimationSrcRect = rect;

	}
}