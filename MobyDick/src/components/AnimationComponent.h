#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include <string>
#include <map>

#include "Component.h"
#include "Animation.h"

class AnimationComponent :  public Component
{
public:
	AnimationComponent();
	~AnimationComponent();

	void update() override;

	//Accessors
	std::string currentAnimationState() {
		return m_currentAnimationState;
	}
	std::map<std::string, Animation*>& animations() {
		return m_animations;
	}
	void setCurrentAnimationState(std::string animationState) { m_currentAnimationState = animationState; }


private:
	std::string
		m_currentAnimationState;

	std::map<std::string, Animation*> m_animations;

};

#endif