#include "SoundManager.h"

#include <fstream>

#include <json/json.h>

#include "GameConfig.h"
#include "game.h"

extern std::unique_ptr<Game>game;


SoundManager::SoundManager()
{

}

SoundManager::~SoundManager()
{
	m_sfxChunks.clear();
	m_sfxMusic.clear();
}

SoundManager& SoundManager::instance()
{
	static SoundManager singletonInstance;
	return singletonInstance;

}

void SoundManager::initSound()
{
	Mix_OpenAudio(48000, AUDIO_F32SYS, 2, 1024);

	auto volume = game->contextMananger()->getSoundVolume();
	Mix_Volume(-1, volume);
	
	loadSounds();
	allocateChannels();

}

void SoundManager::update()
{

	auto volume = game->contextMananger()->getSoundVolume();
	setVolume( volume);

}


void SoundManager::loadSounds()
{

	std::string id, desc, filename;
	Mix_Chunk* soundChunk=nullptr;
	Mix_Music* music = nullptr;

	//Read file and stream it to a JSON object
	Json::Value root;
	std::ifstream ifs("assets/sound/soundAssets.json");
	ifs >> root;

	//Store the sound affects sound items
	for (auto chunksItr : root["soundEffects"])
	{

		id = chunksItr["id"].asString();
		filename = "assets/sound/" + chunksItr["filename"].asString();
		soundChunk = Mix_LoadWAV(filename.c_str());
		m_sfxChunks.emplace(id, soundChunk);

	}

	//Store the sound music items
	for (auto musicItr : root["music"])
	{

		id = musicItr["id"].asString();
		filename = "assets/sound/music/" + musicItr["filename"].asString();
		music = Mix_LoadMUS(filename.c_str());
		m_sfxMusic.emplace(id, music);

	}


}

void SoundManager::stopSound(int channel)
{
	Mix_SetDistance(channel, 0);
	int channelPlayedOn = Mix_HaltChannel(channel);
	std::cout << "Sound Stopped " << std::endl;

}

void SoundManager::stopMusic()
{
	int channelPlayedOn = Mix_HaltMusic();;

}

int SoundManager::playSound(std::string id, int distanceMagnitude, bool loops )
{
	int channelPlayedOn{};
	int loopFlag{};
	if (loops) {
		loopFlag = -1;
	}
	

	//if the distanceMagnitude is 1, then we do not need to set the distance of a particular channel
	if (distanceMagnitude == 1) {

		channelPlayedOn = Mix_PlayChannel(-1, m_sfxChunks[id], loopFlag);
	}
	else {

		int availableChannel = Mix_GroupAvailable(-1);

		if (availableChannel != -1) {
			Mix_SetDistance(availableChannel, distanceMagnitude);

			channelPlayedOn = Mix_PlayChannel(availableChannel, m_sfxChunks[id], loopFlag);
		}
		else {
			std::cout << "negative sound channel for " << id << std::endl;
			return -1;
		}

	}

	return channelPlayedOn;

}


//void SoundManager::playSound(std::string id, int channel, int distanceMagnitude, bool loops)
//{
//
//	int loopFlag{};
//	if (loops) {
//		loopFlag = -1;
//	}
//
//	Mix_SetDistance(channel, distanceMagnitude);
//
//	int channelPlayedOn = Mix_PlayChannel(channel, m_sfxChunks[id], loopFlag);
//
//}

void SoundManager::playMusic(std::string id, int loopTimes)
{
	Mix_PlayMusic(m_sfxMusic[id], loopTimes);

}

void SoundManager::setChannelDistance(int channel, int distance)
{

	Mix_SetDistance(channel, distance);

}

void SoundManager::allocateChannels()
{
	int soundChannels = GameConfig::instance().soundChannels();

	//Mix_AllocateChannels(soundChannels);
	int channels = Mix_AllocateChannels(soundChannels);

}

void SoundManager::setVolume(int volume)
{

	//Indifidual sound effects volume
	Mix_Volume(-1, volume);

	//music volume
	Mix_VolumeMusic(volume);

	//Total sound effects mix volumn - so things dont completely blast speakers when all sounds playing together
	Mix_MasterVolume(int(volume - (volume * .20)));


}

