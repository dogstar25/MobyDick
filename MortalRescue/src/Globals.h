#pragma once
#include <SDL.h>
#include <SDL_image.h>

#include <ctime>
#include <chrono>

using namespace std::chrono;

/*
Overloaded operators used throught the game
*/
static bool operator ==(SDL_Color a, SDL_Color b)
{
	return (a.r == b.r) && (a.g == b.g) && (a.b == b.b);
}

static bool operator !=(SDL_Color a, SDL_Color b)
{
	return (a.r != b.r) || (a.g != b.g) || (a.b != b.b);
}

/*
Clock used throughout the game
FIXME:convert this into a singleton class
*/
struct Clock
{
	uint32_t current_frame_cnt;
	uint32_t fps;
	steady_clock::time_point begin_time;
	steady_clock::time_point end_time;
	std::chrono::duration<double> time_diff;
	std::chrono::duration<double> fps_time_accum;
	std::chrono::duration<double> gameloop_time_accum;

	void init()
	{
		current_frame_cnt = 0;
		fps_time_accum = 0ns;
		begin_time = steady_clock::now();
	}
	void tick()
	{
		end_time = steady_clock::now();
		time_diff = end_time - begin_time;
		begin_time = end_time;

		fps_time_accum += time_diff;
		gameloop_time_accum += time_diff;
	}

	void calcFps()
	{
		//Every 100 frames calculate how long it took to come up with FPS average
		if (current_frame_cnt >= 100)
		{
			fps = current_frame_cnt / fps_time_accum.count();
			fps_time_accum = 0ns;
			current_frame_cnt = 0;
		}
	}

	void resetGameLoopTimeAccum()
	{
		gameloop_time_accum = 0ns;
	}

};


enum GameState {

	QUIT = 0,
	PLAY = 1,
	PAUSE = 2

};

enum GameOjectLayer {

	BACKGROUND = 0,
	MAIN = 1,
	TEXT = 2,
	DEBUG = 3,
};



