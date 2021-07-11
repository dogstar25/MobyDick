#include "AnimationComponent.h"


#include "../EnumMaps.h"
#include "../GameObject.h"


AnimationComponent::AnimationComponent(Json::Value definitionJSON)
{

	//Get reference to the animationComponent JSON config and transformComponent JSON config
	Json::Value animationComponentJSON = definitionJSON["animationComponent"];
	Json::Value transformComponentJSON = definitionJSON["transformComponent"];

	//Build animationComponent details
	m_gameObjectId = definitionJSON["id"].asString();
	
	if (animationComponentJSON.isMember("defaultState")) {
		m_defaultAnimationState = EnumMap::instance().toEnum(animationComponentJSON["defaultState"].asString());
	}
	else {
		m_defaultAnimationState=0;
	}

	int i = 0;
	for (Json::Value animItr : animationComponentJSON["animations"])
	{
		i++;
		int state = EnumMap::instance().toEnum(animItr["state"].asString());

		//Initialze current animation state to the first animation in the list
		if (i == 1) {
			m_currentAnimationState = state;
		}

		m_animations[state] = Animation(animItr, transformComponentJSON);

	}
}



AnimationComponent::~AnimationComponent()
{


}

void AnimationComponent::update()
{

	//std::cout << "Animating state " << m_currentAnimationState << "\n";
	auto animationFrame = m_animations[m_currentAnimationState].animate();
	//m_animations[m_currentAnimationState].animate();

	//If this animation has completed and it was a one-time animate, then reset the current
	//animation to the default, and put it in continuous mode (probably IDLE)
	if (animationFrame == 0) {

		if (m_currentAnimationMode == ANIMATE_ONE_TIME) {
			m_currentAnimationState = m_defaultAnimationState;
			m_currentAnimationMode = ANIMATE_CONTINUOUS;
		}
	}

}

void AnimationComponent::animate(int animationState, int animationMode)
{

	m_currentAnimationState = animationState;
	m_currentAnimationMode = animationMode;

}


SDL_Rect* AnimationComponent::getCurrentAnimationTextureRect()
{
	//assert(m_animations.find(m_currentAnimationState) != m_animations.end() && "Animation State not found in animations collection");

	const auto& animationTextureRect = m_animations[m_currentAnimationState].getCurrentTextureAnimationSrcRect();
	return animationTextureRect.get();

}

SDL_Texture* AnimationComponent::getCurrentAnimationTexture()
{
	SDL_Texture* texture = nullptr;

	//assert(m_animations.find(m_currentAnimationState) != m_animations.end() && "Animation State not found in animations collection");
	texture = m_animations.at(m_currentAnimationState).getTexture();

	return texture;

}

void AnimationComponent::setDefaultAnimationState(int defaultAnimationState)
{
	m_defaultAnimationState = defaultAnimationState;
}



