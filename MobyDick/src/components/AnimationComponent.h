#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include <string>
#include <map>
#include <json/json.h>

#include "Component.h"
#include "../Animation.h"

class TransformComponent;

class AnimationComponent :  public Component
{
public:
	AnimationComponent();
	AnimationComponent(std::string gameObjectId);
	AnimationComponent(Json::Value itrJSON);
	~AnimationComponent();
	void construct(Json::Value itrJSON);

	void update() override;
	SDL_Rect* getCurrentAnimationTextureRect();
	SDL_Texture* getCurrentAnimationTexture();

	//Accessors
	int currentAnimationState() {
		return m_currentAnimationState;
	}
	std::map<int, std::shared_ptr<Animation>>& animations() {
		return m_animations;
	}
	void setCurrentAnimationState(int animationState) { m_currentAnimationState = animationState; }
	void setDependencyReferences(std::shared_ptr<TransformComponent> transformComponent);

private:
	int 
		m_currentAnimationState;

	std::shared_ptr<TransformComponent> m_refTransFormComponent;
	std::map<int, std::shared_ptr<Animation>> m_animations;

};

#endif