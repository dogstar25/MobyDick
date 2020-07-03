#include "AnimationComponent.h"

#include "../EnumMaps.h"
#include "../GameObjectManager.h"
#include "../GameObject.h"


AnimationComponent::AnimationComponent()
{
}

AnimationComponent::AnimationComponent(std::string gameObjectId, std::shared_ptr<GameObject> parentGameObject)
{
	Json::Value itrJSON = GameObjectManager::instance().getDefinition(gameObjectId)->definitionJSON();

	//Transform Component
	if (itrJSON.isMember("animationComponent"))
	{
		m_parentGameObject = parentGameObject;
		m_parentGameObject->componentFlags().set(ANIMATION_COMPONENT);

		m_currentAnimationState = 1;

		int i = 0;
		for (Json::Value animItr : itrJSON["animationComponent"]["animations"])
		{
			i++;
			int state = EnumMap::instance().toEnum(animItr["state"].asString());

			//Initialze curretn animation state to the first animation in the list
			if (i == 1) {
				m_currentAnimationState = state;
			}
			//Animation* animation = new Animation(animItr);
			m_animations.emplace(state, *(new Animation(animItr)));

		}

	}

}

AnimationComponent::~AnimationComponent()
{

}

void AnimationComponent::update()
{

}


SDL_Rect* AnimationComponent::getCurrentAnimationTextureRect()
{
	SDL_Rect* textureSrcRect = nullptr;

	textureSrcRect =
		m_animations[m_currentAnimationState].getCurrentTextureAnimationSrcRect();

	return textureSrcRect;
	
}

SDL_Texture* AnimationComponent::getCurrentAnimationTexture()
{
	SDL_Texture* texture = nullptr;

	texture =
		m_animations[m_currentAnimationState].getTexture();

	return texture;

}





