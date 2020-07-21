#include "AnimationComponent.h"
#include "TransformComponent.h"
#include "../EnumMaps.h"


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




