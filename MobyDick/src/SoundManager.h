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
	void update();
	int playSound(std::string id);
	void playSound(std::string id, int channel);
	void playMusic(std::string id, int loopTimes);
	void stopSound(int channel);
	void setVolume(int volume);

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