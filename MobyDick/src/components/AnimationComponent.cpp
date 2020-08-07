#include "AnimationComponent.h"

#include <iostream>

#include "../EnumMaps.h"
#include "../GameObjectManager.h"
#include "../GameObject.h"

#include "TransformComponent.h"


AnimationComponent::AnimationComponent()
{
}

AnimationComponent::AnimationComponent(Json::Value definitionJSON)
{

	//Get reference to the animationComponent JSON config and transformComponent JSON config
	Json::Value animationComponentJSON = definitionJSON["animationComponent"];
	Json::Value transformComponentJSON = definitionJSON["transformComponent"];

	//Build animationComponent details
	m_gameObjectId = definitionJSON["id"].asString();
	
	if (definitionJSON.isMember("defaultState")) {
		m_defaultAnimationState = EnumMap::instance().toEnum(definitionJSON["defaultState"].asString());
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
		//Animation* animation = new Animation(animItr);
		m_animations.emplace(state, new Animation(animItr, transformComponentJSON));

	}
}



AnimationComponent::~AnimationComponent()
{

	m_animations.clear();

}

void AnimationComponent::update()
{

	std::cout << "Animating state " << m_currentAnimationState << "\n";
	auto animationFrame = m_animations[m_currentAnimationState]->animate();

	//If this animation has completed and it was a one-time animate, then reset the current
	//animation to the deafult in continuous mode
	if (animationFrame == 0) {

		std::cout << "reset state " << m_currentAnimationState << "\n";
		if (m_currentAnimationMode == ANIMATE_ONE_TIME) {
			m_currentAnimationMode = ANIMATE_CONTINUOUS;
			m_currentAnimationState = m_defaultAnimationState;
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
	//SDL_Rect* textureSrcRect = nullptr;

	return 
		m_animations[m_currentAnimationState]->getCurrentTextureAnimationSrcRect().get();

	//return textureSrcRect;
	
}

SDL_Texture* AnimationComponent::getCurrentAnimationTexture()
{
	SDL_Texture* texture = nullptr;

	texture =
		m_animations[m_currentAnimationState]->getTexture();

	return texture;

}





