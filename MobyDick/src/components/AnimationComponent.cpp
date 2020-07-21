#include "AnimationComponent.h"
#include "TransformComponent.h"
#include "../TextureManager.h"
#include "../EnumMaps.h"
#include <json/json.h>


Animation::Animation(Json::Value animationDetailsJSON, Json::Value transformDetailsJSON)
{

	m_currentAnimFrame = 0;
	m_state = EnumMap::instance().toEnum(animationDetailsJSON["state"].asString());
	m_speed = animationDetailsJSON["speed"].asFloat();

	m_frameCount = animationDetailsJSON["frames"].asInt();

	//Get texture
	std::string textureId = animationDetailsJSON["textureId"].asString();

	m_texture = TextureManager::instance().getTexture(textureId);
	m_currentTextureAnimationSrcRect = nullptr;

	//Calculate how many columns and rows this animation texture has
	int width, height;
	//First get width of textture
	SDL_QueryTexture(m_texture->sdlTexture, NULL, NULL, &width, &height);

	m_frameSize.x = transformDetailsJSON["size"]["width"].asFloat();
	m_frameSize.y = transformDetailsJSON["size"]["height"].asFloat();

	//Calculate nnumber of rows and columns - remember to convert the gameObject size to pixels first
	int rows, columns;
	columns = width / m_frameSize.x;
	rows = height / m_frameSize.y;
	//	//Calculate top left corner of each animation frame
	SDL_FPoint point;
	int frameCount = 0;
	for (int rowIdx = 0; rowIdx < rows; rowIdx++)
	{
		for (int colIdx = 0; colIdx < columns; colIdx++)
		{
			point.x = colIdx * m_frameSize.x;
			point.y = rowIdx * m_frameSize.y;
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

AnimationComponent::AnimationComponent()
{
}

AnimationComponent::AnimationComponent(Json::Value definitionJSON)
{

	//Get reference to the animationComponent JSON config and transformComponent JSON config
	Json::Value animationComponentJSON = definitionJSON["animationComponent"];
	Json::Value transformComponentJSON = definitionJSON["transformComponent"];

	int i = 0;
	for (Json::Value animItr : animationComponentJSON["animations"])
	{
		i++;
		int state = EnumMap::instance().toEnum(animItr["state"].asString());

		//Initialze current animation state to the first animation in the list
		if (i == 1) {
			m_currentAnimationState = state;
		}
		//Animation* animation = new Animation(animItr);
		m_animations.emplace(state, new Animation(animItr, transformComponentJSON));

	}
}




