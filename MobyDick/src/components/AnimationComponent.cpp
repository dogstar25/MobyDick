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

	//Save the pointer to parent GameObject
	m_parentGameObject = parentGameObject;

	//Animation Component - requires transform component as well 
	if (itrJSON.isMember("animationComponent") && itrJSON.isMember("transformComponent"))
	{
		m_parentGameObject->setComponentFlag(ANIMATION_COMPONENT);

		//Get reference to the animationComponent JSON config and transformComponent JSON config
		Json::Value animationComponentJSON = itrJSON["animationComponent"];
		Json::Value transformComponentJSON = itrJSON["transformComponent"];

		//Build animationComponent details
		m_currentAnimationState = 1;

		int i = 0;
		for (Json::Value animItr : animationComponentJSON["animations"])
		{
			i++;
			int state = EnumMap::instance().toEnum(animItr["state"].asString());

			//Initialze curretn animation state to the first animation in the list
			if (i == 1) {
				m_currentAnimationState = state;
			}
			//Animation* animation = new Animation(animItr);
			m_animations.emplace(state, new Animation(animItr, transformComponentJSON));

		}

	}

}

AnimationComponent::~AnimationComponent()
{

	m_animations.clear();
	//std::map<int, Animation*>().swap(m_animations);

}

void AnimationComponent::update()
{

	m_animations[m_currentAnimationState]->animate();

}


SDL_Rect* AnimationComponent::getCurrentAnimationTextureRect()
{
	SDL_Rect* textureSrcRect = nullptr;

	textureSrcRect =
		m_animations[m_currentAnimationState]->getCurrentTextureAnimationSrcRect();

	return textureSrcRect;
	
}

SDL_Texture* AnimationComponent::getCurrentAnimationTexture()
{
	SDL_Texture* texture = nullptr;

	texture =
		m_animations[m_currentAnimationState]->getTexture();

	return texture;

}





