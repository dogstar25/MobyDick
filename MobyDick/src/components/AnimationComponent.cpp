#include "AnimationComponent.h"
#include "RenderComponent.h"
#include "../game.h"


#include "../EnumMap.h"

extern std::unique_ptr<Game> game;

AnimationComponent::AnimationComponent(Json::Value componentJSON)
{
	m_componentType = ComponentTypes::ANIMATION_COMPONENT;

	if (componentJSON.isMember("defaultState")) {
		m_defaultAnimationState = game->enumMap()->toEnum(componentJSON["defaultState"].asString());
	}
	else {
		m_defaultAnimationState=0;
	}

	m_frameSize.x = componentJSON["frameSize"]["width"].asFloat();
	m_frameSize.y = componentJSON["frameSize"]["height"].asFloat();

	int i = 0;
	for (Json::Value animItr : componentJSON["animations"])
	{
		i++;
		int state = game->enumMap()->toEnum(animItr["state"].asString());

		//Initialze current animation state to the first animation in the list
		if (i == 1) {
			m_currentAnimationState = state;
		}

		m_animations[state] = Animation(animItr, m_frameSize);

	}
}



AnimationComponent::~AnimationComponent()
{


}

void AnimationComponent::update()
{

	auto animationFrame = m_animations[m_currentAnimationState].animate(parent());

	//If this animation has completed and it was a one-time animate, then reset the current
	//animation to the default, and put it in continuous mode (probably IDLE)
		if (animationFrame == 0) {

		if (m_currentAnimationMode == ANIMATE_ONE_TIME) {
			m_currentAnimationState = m_defaultAnimationState;
			m_currentAnimationMode = ANIMATE_CONTINUOUS;
		}
	}

	//Should we flash?
	if (m_flashAnimation.has_value()) {

		const auto& renderComponent = parent()->getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT);

		//Have we flashed the correct number of times
		if (m_flashAnimation.value().flashCount <= m_flashAnimation.value().flashTimes) {

			//Flash at the defined speed
			if (m_flashAnimation.value().speedTimer.hasMetTargetDuration()) {

				//If the flashStatus is ON, then remove the color treatment, otherwise add it
				if (m_flashAnimation.value().flashFlag == FlashFlag::flashOFF) {

					//Build a render overlay to apply to the render component
					DisplayOverlay displayOverlay{};
					displayOverlay.color = m_flashAnimation.value().flashColor;
					displayOverlay.color.value().a = m_flashAnimation.value().flashAlpha;

					renderComponent->applyDisplayOverlay(displayOverlay);

					m_flashAnimation.value().flashCount += 1;

					m_flashAnimation.value().flashFlag = FlashFlag::flashON;

				}
				else {
					renderComponent->removeDisplayOverlay();
					m_flashAnimation.value().flashFlag = FlashFlag::flashOFF;
				}

				//reset flashTimer
				m_flashAnimation.value().speedTimer.reset();
			}

		}
		else {
			renderComponent->removeDisplayOverlay();
			m_flashAnimation.reset();
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

std::shared_ptr<Texture> AnimationComponent::getCurrentAnimationTexture()
{

	//assert(m_animations.find(m_currentAnimationState) != m_animations.end() && "Animation State not found in animations collection");
	return m_animations.at(m_currentAnimationState).getTexture();


}

void AnimationComponent::setDefaultAnimationState(int defaultAnimationState)
{
	m_defaultAnimationState = defaultAnimationState;
}

void AnimationComponent::setFlash(SDL_Color flashColor, float flashSpeed, int flashTimes)
{
	FlashAnimation flashAnimation;
	flashAnimation.flashColor = flashColor;
	flashAnimation.flashSpeed = flashSpeed;
	flashAnimation.flashTimes = flashTimes;
	flashAnimation.flashCount = 0;
	flashAnimation.speedTimer = { flashSpeed, true };
	m_flashAnimation = std::move(flashAnimation);
}



