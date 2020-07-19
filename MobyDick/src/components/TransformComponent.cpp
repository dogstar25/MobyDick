#include "TransformComponent.h"
#include "../GameConfig.h"
#include "../DebugPanel.h"


TransformComponent::TransformComponent()
{
}

TransformComponent::TransformComponent(Json::Value definitionJSON, float xMapPos, float yMapPos, float angleAdjust)
{
		Json::Value transformComponentJSON = definitionJSON["transformComponent"];

		m_angle = angleAdjust;

		//FixMe: 32 is tilewidth
		m_position = {
			(xMapPos * 32) + transformComponentJSON["size"]["width"].asFloat() / 2,
			(yMapPos * 32) + transformComponentJSON["size"]["height"].asFloat() / 2
		};

		m_originalPosition = m_position;
		m_size.Set(transformComponentJSON["size"]["width"].asFloat(), transformComponentJSON["size"]["height"].asFloat());

		m_absolutePositioning = transformComponentJSON["absolutePositioning"].asBool();

}


SDL_FRect TransformComponent::getPositionRect()
{
	SDL_FRect positionRect;

	positionRect.w = m_size.x;
	positionRect.h = m_size.y;

	positionRect.x = m_position.x;
	positionRect.y = m_position.y;

	return positionRect;

}


