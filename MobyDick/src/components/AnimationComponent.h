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
	AnimationComponent(Json::Value definitionJSON);
	~AnimationComponent();


	int m_currentAnimationState;
	std::map<int, std::shared_ptr<Animation>> m_animations;

	SDL_Rect* getCurrentAnimationTextureRect();
	SDL_Texture* getCurrentAnimationTexture();

};

#endif