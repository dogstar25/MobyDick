#include "AnimationComponent.h"

#include "../EnumMaps.h"
#include "../GameObjectManager.h"
#include "../GameObject.h"

#include "TransformComponent.h"


AnimationComponent::AnimationComponent()
{
}

AnimationComponent::AnimationComponent(Json::Value definitionJSON)
{

	//new direction?
	//m_gameObject = std::make_shared<GameObject>(*gameObject);


	//Get reference to the animationComponent JSON config and transformComponent JSON config
	Json::Value animationComponentJSON = definitionJSON["animationComponent"];
	Json::Value transformComponentJSON = definitionJSON["transformComponent"];

	//Build animationComponent details
	m_gameObjectId = definitionJSON["id"].asString();;

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


void AnimationComponent::setDependencyReferences(std::shared_ptr<TransformComponent> transformComponent)
{
	m_refTransFormComponent = std::shared_ptr<TransformComponent>(transformComponent);
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





