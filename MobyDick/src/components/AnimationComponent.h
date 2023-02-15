#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include <string>
#include <map>
#include <memory>
#include <json/json.h>

#include <memory>
#include <SDL2/SDL.h>

#include "Component.h"
#include "../Animation.h"
#include "../texture.h"

class TransformComponent;

enum class FlashFlag {
	flashON,
	flashOFF
};

struct FlashAnimation
{
	SDL_Color flashColor{};
	int flashAlpha{255};
	float flashSpeed{};
	int flashTimes{};
	int flashCount{0};
	Timer speedTimer{};
	FlashFlag flashFlag { FlashFlag::flashOFF };
};

class AnimationComponent : public Component
{
public:
	AnimationComponent() {};
	AnimationComponent(Json::Value definitionJSON);
	~AnimationComponent();

	void update() override;
	SDL_Rect* getCurrentAnimationTextureRect();
	std::shared_ptr<Texture> getCurrentAnimationTexture();
	void animate(int animationState, int animationMode);

	void setCurrentAnimationState(int animationState) { m_currentAnimationState = animationState; }
	int currentAnimationState() { return m_currentAnimationState; }
	int defaultAnimationState() { return m_defaultAnimationState; }
	void setDefaultAnimationState(int defaultAnimationState);
	void setToDefaultAnimation();
	void setFlash(SDL_Color flashColor, float flashSpeed, int flashTimes);

	std::array<Animation, MAX_ANIMATION_STATES>& animations() { return m_animations; }

private:

	int m_currentAnimationState {};
	int m_currentAnimationMode { ANIMATE_ONE_TIME };
	int m_defaultAnimationState {};
	b2Vec2 m_frameSize{};
	std::array<Animation, MAX_ANIMATION_STATES> m_animations;

	std::optional<FlashAnimation> m_flashAnimation{};
};

#endif