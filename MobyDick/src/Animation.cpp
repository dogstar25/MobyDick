#include "Animation.h"

#include <iostream>

#include "EnumMaps.h"
#include "TextureManager.h"


Animation::Animation(Json::Value animationDetailsJSON, b2Vec2 frameSize) :
	m_frameSize(frameSize)
{

	m_currentAnimFrame = 0;
	m_state = EnumMap::instance().toEnum(animationDetailsJSON["state"].asString());
	m_speed = animationDetailsJSON["speed"].asFloat();

	m_timer = Timer(m_speed);

	m_frameCount = animationDetailsJSON["frames"].asInt();

	//Get texture
	std::string textureId = animationDetailsJSON["textureId"].asString();

	m_texture = TextureManager::instance().getTexture(textureId);
	m_currentTextureAnimationSrcRect = nullptr;

	//Calculate how many columns and rows this animation texture has
	int rows, columns;
	columns = (int)(m_texture->textureAtlasQuad.w / m_frameSize.x);
	rows = (int)(m_texture->textureAtlasQuad.h / m_frameSize.y);

	//Calculate top left corner of each animation frame
	SDL_FPoint point;
	int frameCount = 0;
	for (int rowIdx = 0; rowIdx < rows; rowIdx++) 
	{
		for (int colIdx = 0; colIdx < columns; colIdx++) 
		{
			point.x = m_texture->textureAtlasQuad.x + (colIdx * m_frameSize.x);
			point.y = m_texture->textureAtlasQuad.y + (rowIdx * m_frameSize.y);
			m_animationFramePositions.push_back(point);
			//do not exceed the maximum number of frames that this texture holds
			frameCount++;
			if (frameCount >= animationDetailsJSON["frames"].asInt())
			{
				break;
			}
		}
	}

}




int Animation::animate()
{
	//check the clock and see if enough time as gone by

	if (m_timer.hasMetTargetDuration())
	{

		//Increment animation frame counter and reset if it exceeds last one
		if (m_frameCount > 1) {
			this->m_currentAnimFrame += 1;
		}
		
		if (this->m_currentAnimFrame >
			m_frameCount - 1) {

			this->m_currentAnimFrame = 0;
		}

		//build the rectangle that points to the current animation frame
		std::shared_ptr<SDL_Rect> rect = std::make_shared<SDL_Rect>();

		rect->x = (int)m_animationFramePositions[m_currentAnimFrame].x;
		rect->y = (int)m_animationFramePositions[m_currentAnimFrame].y;

		rect->w = (int)m_frameSize.x;
		rect->h = (int)m_frameSize.y;

		m_currentTextureAnimationSrcRect = rect;

	}

	return this->m_currentAnimFrame;
}

