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
	Mix_OpenAudio(44100, AUDIO_S32SYS, 2, 4096);

	auto volume = game->contextMananger()->getSoundVolume();
	Mix_Volume(-1, volume);
	
	loadSounds();
	allocateChannels();

}

void SoundManager::update()
{

	auto volume = game->contextMananger()->getSoundVolume();
	Mix_Volume(-1, volume);

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
		filename = "assets/sound/" + musicItr["filename"].asString();
		music = Mix_LoadMUS(filename.c_str());
		m_sfxMusic.emplace(id, music);

	}


}

void SoundManager::stopSound(int channel)
{
	int channelPlayedOn = Mix_HaltChannel(channel);

}

int SoundManager::playSound(std::string id )
{

	//This should return an avaialable channel from the default group (all channels)
	int availableChannel = Mix_GroupAvailable(-1);

	//can add a distance input now, and use teh avail channel
	//?????
	//actually, should the sound played ALWAYS be the distance from the player?ok, is he working on it?




	int channelPlayedOn = Mix_PlayChannel(availableChannel, m_sfxChunks[id], 0);
	return channelPlayedOn;

}


void SoundManager::playSound(std::string id, int channel)
{
	int channelPlayedOn = Mix_PlayChannel(channel, m_sfxChunks[id], 0);

}

void SoundManager::playMusic(std::string id, int loopTimes)
{
	Mix_PlayMusic(m_sfxMusic[id], loopTimes);

}

void SoundManager::allocateChannels()
{
	int soundChannels = GameConfig::instance().soundChannels();

	//Mix_AllocateChannels(soundChannels);
	int channels = Mix_AllocateChannels(soundChannels);

}

void SoundManager::setVolume(int volume)
{
	Mix_Volume(-1, volume);
}

