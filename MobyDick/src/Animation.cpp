#include "Animation.h"

#include "EnumMaps.h"


Animation::Animation()
{
}

Animation::Animation(Json::Value animationDetailsJSON)
{
	//Convenience reference
	Json::Value& itr = animationDetailsJSON;

	m_currentAnimFrame = 0;
	m_texture = nullptr;
	m_currentTextureAnimationSrcRect = nullptr;
	m_state = EnumMap::instance().toEnum(itr["state"].asString());
	m_speed = itr["speed"].asFloat();
	m_frameCount = itr["frames"].asInt();


}

//Animation::Animation(GameObjectDefinition* gameObjectDefinition, std::string animationId, std::string textureId, int totalFrames, float speed)
//{
//
//	//m_state = animationId;
//	m_speed = speed;
//	m_frameCount = totalFrames;
//
//	//Get pointer to textture
//	//FIXME:Change this to simple use the 
//	m_texture = TextureManager::instance().getTexture(textureId)->sdlTexture;
//
//	//Calculate how many columns and rows this animation texture has
//	int width, height;
//	//First get width of textture
//	SDL_QueryTexture(m_texture, NULL, NULL, &width, &height);
//
//	//calculate frameSize
//	if (gameObjectDefinition->isPhysicsObject == true)
//	{
//		m_frameSize.x = gameObjectDefinition->xSize * GameConfig::instance().scaleFactor();
//		m_frameSize.y = gameObjectDefinition->ySize * GameConfig::instance().scaleFactor();
//	}
//	else
//	{
//		m_frameSize.x = gameObjectDefinition->xSize;
//		m_frameSize.y = gameObjectDefinition->ySize;
//
//	}
//
//	//Calculate nnumber of rows and columns - remember to convert the gameObject size to pixels first
//	int rows, columns;
//	columns = width / m_frameSize.x;
//	rows = height / m_frameSize.y;
//
//	//Calculate top left corner of each animation frame
//	SDL_FPoint point;
//	int frameCount = 0;
//	for (int rowIdx = 0; rowIdx < rows; rowIdx++) 
//	{
//		for (int colIdx = 0; colIdx < columns; colIdx++) 
//		{
//
//			point.x = colIdx * m_frameSize.x;
//			point.y = rowIdx * m_frameSize.y;
//
//			m_animationFramePositions.push_back(point);
//
//			//do not exceed the maximum number of frames that this texture holds
//			frameCount++;
//			if (frameCount >= totalFrames) {
//				break;
//			}
//		}
//	}
//}

Animation::~Animation()
{

	delete m_currentTextureAnimationSrcRect;
	delete m_texture;

}

void Animation::animate()
{
	//check the clock and see if enough time as gone by
	std::chrono::steady_clock::time_point now_time = std::chrono::steady_clock::now();
	std::chrono::duration<double> time_diff = now_time - m_timeSnapshot;

	if (time_diff.count() >= m_speed)
	{
		m_timeSnapshot = now_time;

		//Increment animation frame counter and reset if it exceeds last one
		this->m_currentAnimFrame += 1;
		if (this->m_currentAnimFrame >
			m_frameCount - 1) {

			this->m_currentAnimFrame = 0;
		}

		//build the rectangle that points to the current animation frame
		SDL_Rect* rect = new SDL_Rect();

		rect->x = m_animationFramePositions[m_currentAnimFrame].x;
		rect->y = m_animationFramePositions[m_currentAnimFrame].y;

		rect->w = m_frameSize.x;
		rect->h = m_frameSize.y;

		m_currentTextureAnimationSrcRect = rect;

	}
}

