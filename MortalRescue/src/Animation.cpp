#include <Box2D/Box2D.h>

#include "Animation.h"
#include "GameObject.h"
#include "GameObjectDefinition.h"
#include "game.h"

Animation::Animation(GameObjectDefinition* gameObjectDefinition, string id, string textureId, int totalFrames, float speed)
{
	this->id = id;
	this->speed = speed;
	this->frameCount = totalFrames;

	//Get pointer to textture
	texture = game->textureManager.getTexture(textureId)->sdlTexture;

	//Calculate how many columns and rows this animation texture has
	int width, height;
	//First get width of textture
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);

	//calculate frameSize
	if (gameObjectDefinition->isPhysicsObject == true)
	{
		this->frameSize.x = gameObjectDefinition->xSize * game->config.scaleFactor;
		this->frameSize.y = gameObjectDefinition->ySize * game->config.scaleFactor;
	}
	else
	{
		this->frameSize.x = gameObjectDefinition->xSize;
		this->frameSize.y = gameObjectDefinition->ySize;

	}

	//Calculate nnumber of rows and columns - remember to convert the gameObject size to pixels first
	int rows, columns;
	columns = width / this->frameSize.x;
	rows = height / this->frameSize.y;

	//Calculate top left corner of each animation frame
	SDL_Point point;
	int frameCount = 0;
	for (int rowIdx = 0; rowIdx < rows; rowIdx++) {
		for (int colIdx = 0; colIdx < columns; colIdx++) {

			point.x = colIdx * this->frameSize.x;
			point.y = rowIdx * this->frameSize.y;

			animationFramePositions.push_back(point);

			//do not exceed the maximum number of frames that this texture holds
			frameCount++;
			if (frameCount >= totalFrames) {
				break;
			}
		}
	}
}

Animation::~Animation()
{

	delete this->currentTextureAnimationSrcRect;
	delete this->texture;

}

SDL_Rect* Animation::getRenderTextureRect()
{

	return this->currentTextureAnimationSrcRect;

}

SDL_Texture* Animation::getRenderTexture()
{

	return this->texture;

}

void Animation::animate()
{
	//check the clock and see if enough time as gone by
	steady_clock::time_point now_time = steady_clock::now();
	std::chrono::duration<double> time_diff = now_time - this->time_snapshot;

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
		SDL_Rect* rect = new SDL_Rect();

		rect->x = this->animationFramePositions[currentAnimFrame].x;
		rect->y = this->animationFramePositions[currentAnimFrame].y;

		rect->w = this->frameSize.x;
		rect->h = this->frameSize.y;

		this->currentTextureAnimationSrcRect = rect;

	}
}

