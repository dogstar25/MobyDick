#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H


#include <string>
#include <stdio.h>
#include <map>
#include <memory>

#include <SDL2/SDL_mixer.h>



class SoundManager
{

public:
	static SoundManager& instance();
	void initSound();
	void playSound(std::string id);
	void playMusic(std::string id, int loopTimes);

	float test;
	

private:
	SoundManager();
	~SoundManager();

	std::map<std::string, Mix_Chunk*> m_sfxChunks;
	std::map<std::string, Mix_Music*> m_sfxMusic;

	void loadSounds();
	void allocateChannels();


};

#endif