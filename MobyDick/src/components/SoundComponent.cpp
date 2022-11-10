#include "SoundComponent.h"
#include "../SoundManager.h"
#include "../LevelManager.h"

#include <memory.h>

#include "../EnumMap.h"
#include "../game.h"


extern std::unique_ptr<Game> game;

SoundComponent::SoundComponent(Json::Value componentJSON)
{
	m_componentType = ComponentTypes::SOUND_COMPONENT;

	for (Json::Value itr: componentJSON["sounds"])
	{
		SoundItem soundItem{};
		soundItem.id = itr["id"].asString();
		soundItem.soundAssetId = itr["soundAssetId"].asString();
		soundItem.isDistanceSensitive = itr["distanceSensitive"].asBool();
		soundItem.isContinuous = itr["continuous"].asBool();
		soundItem.soundRange = itr["soundRange"].asInt();

		m_sounds.emplace(soundItem.id, soundItem);

	}

}

SoundComponent::~SoundComponent()
{

	stopSounds();
	m_sounds.clear();

}

void SoundComponent::update()
{

	//Loop through all of the sounds and for ones that are continuous and distance sensitive, 
	//update the sound channel that its on with the distance from this object to the player
	auto itr = m_sounds.begin();
	while (itr != m_sounds.end()) {

		if (itr->second.isDistanceSensitive == true && itr->second.isContinuous == true) {

			const auto playerPosition = parent()->parentScene()->getGameObjectsByTrait(TraitTag::player)[0]->getCenterPosition();
			const auto parentPosition = parent()->getCenterPosition();
			int soundDistanceMagnitude = _calculateSoundDistanceMagnitude(playerPosition, parentPosition, itr->second.soundRange);

			SoundManager::instance().setChannelDistance(itr->second.soundChannel, soundDistanceMagnitude);
			
		}

		++itr;

	}



}

void SoundComponent::postInit()
{

	//Loop though and find all sounds that are continuous and start them up and save their channel
	auto itr = m_sounds.begin();
	while (itr != m_sounds.end()) {

		if (itr->second.isContinuous == true) {

			SoundManager::instance().setChannelDistance(itr->second.soundChannel, 255);
			int channel = SoundManager::instance().playSound(itr->second.soundAssetId, 255, true);
			itr->second.soundChannel = channel;

		}

		++itr;
	}


}

void SoundComponent::stopSounds()
{

	//Loop though and find all sounds that are continuous and stop them from playing to free up the channel
	auto itr = m_sounds.begin();
	while (itr != m_sounds.end()) {

		if (itr->second.isContinuous == true) {

			stopSound(itr->second.soundChannel);

		}

		++itr;
	}

}

void SoundComponent::stopSound(int channel)
{

	SoundManager::instance().stopSound(channel);

}

void SoundComponent::setParent(GameObject* gameObject)
{
	//Call base component setParent
	Component::setParent(gameObject);

}

int SoundComponent::playSound(std::string soundId)
{
	int channel{};
	int soundDistanceMagnitude{};

	if (m_sounds.find(soundId) != m_sounds.end()) {

		if (m_sounds.at(soundId).isDistanceSensitive) {

			const auto playerPosition = parent()->parentScene()->getGameObjectsByTrait(TraitTag::player)[0]->getCenterPosition();
			const auto parentPosition = parent()->getCenterPosition();

			soundDistanceMagnitude =  _calculateSoundDistanceMagnitude(playerPosition, parentPosition, (m_sounds.at(soundId).soundRange));
		}

		channel = SoundManager::instance().playSound(m_sounds.at(soundId).soundAssetId, soundDistanceMagnitude);

	}

	return channel;

}

int SoundComponent::_calculateSoundDistanceMagnitude(SDL_FPoint playerPosition, SDL_FPoint parentPosition, int soundRange)
{

	float playerToSoundSource = util::calculateDistance(playerPosition, parentPosition);

	float soundMagnitude = (playerToSoundSource / soundRange) * 255.0;
	soundMagnitude = std::min((int)soundMagnitude, 255);

	
	return soundMagnitude;
}




