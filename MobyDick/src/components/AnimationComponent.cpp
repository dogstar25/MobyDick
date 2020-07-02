#include "AnimationComponent.h"

#include "../EnumMaps.h"




AnimationComponent::AnimationComponent(Json::Value& componentDetailsJSON)
{
	//Convenience reference
	Json::Value& itr = componentDetailsJSON;

	
	int i = 0;
	for (auto animItr : itr["animations"])
	{
		i++;
		int state = animItr["state"].asInt();

		//Initialze curretn animation state to the first animation in the list
		if (i == 1) {
			m_currentAnimationState = state;
		}
		m_animations.emplace(state, new Animation(animItr));

	}

}

AnimationComponent::~AnimationComponent()
{

}





