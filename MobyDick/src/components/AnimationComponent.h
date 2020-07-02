#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include <string>
#include <map>
#include <json/json.h>

#include "Component.h"
#include "Animation.h"

class AnimationComponent :  public Component
{
public:
	AnimationComponent(Json::Value& componentDetailsJSON);
	~AnimationComponent();

	void update() override;

	//Accessors
	int currentAnimationState() {
		return m_currentAnimationState;
	}
	std::map<int, Animation>& animations() {
		return m_animations;
	}
	void setCurrentAnimationState(std::string animationState) { m_currentAnimationState = animationState; }


private:
	int 
		m_currentAnimationState;

	std::map<int, Animation> m_animations;

};

#endif