#include "SoundManager.h"

#include <iostream>
#include <fstream>

#include <json/json.h>

#include "Game.h"



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
	Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512);

	loadSounds();
	allocateChannels();

}


void SoundManager::loadSounds()
{

	string id, desc, filename;
	Mix_Chunk* soundChunk=nullptr;
	Mix_Music* music = nullptr;

	//Read file and stream it to a JSON object
	Json::Value root;
	ifstream ifs("assets/sound/soundAssets.json");
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

void SoundManager::playSound(string id )
{
	
	int channelPlayedOn = Mix_PlayChannel(-1, m_sfxChunks[id], 0);

}

void SoundManager::playMusic(string id, int loopTimes)
{

	Mix_PlayMusic(m_sfxMusic[id], loopTimes);

}

void SoundManager::allocateChannels()
{
	int soundChannels = game->config.soundChannels;

	Mix_AllocateChannels(soundChannels);

}
