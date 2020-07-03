#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include <string>
#include <map>
#include <json/json.h>

#include "Component.h"
#include "../Animation.h"

class GameObject;

class AnimationComponent :  public Component
{
public:
	AnimationComponent();
	AnimationComponent(std::string gameObjectId, std::shared_ptr<GameObject> parentGameObject);
	~AnimationComponent();

	void update() override;
	SDL_Rect* getCurrentAnimationTextureRect();
	SDL_Texture* getCurrentAnimationTexture();

	//Accessors
	int currentAnimationState() {
		return m_currentAnimationState;
	}
	std::map<int, Animation>& animations() {
		return m_animations;
	}
	void setCurrentAnimationState(int animationState) { m_currentAnimationState = animationState; }


private:
	int 
		m_currentAnimationState;

	std::map<int, Animation> m_animations;

};

#endif