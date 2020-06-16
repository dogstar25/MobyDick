#pragma once

#include <string>
#include <stdio.h>
#include <map>
#include <memory>


#include <SDL_mixer.h>

using namespace std;

class SoundManager
{

public:
	static SoundManager& instance();
	void initSound();
	void playSound(string id);
	void playMusic(string id, int loopTimes);


private:
	SoundManager();
	~SoundManager();

	map<string, Mix_Chunk*> m_sfxChunks;
	map<string, Mix_Music*> m_sfxMusic;

	void loadSounds();
	void allocateChannels();


};

