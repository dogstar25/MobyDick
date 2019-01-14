#ifndef __Game__
#define __Game__

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include <json/json.h>
#include <iostream>
#include <fstream>
#include "game.h"
#include <stdio.h>
#include <ctime>
#include <ratio>
#include <chrono>
#include <Box2D/Box2D.h>
#include "Util.h"


using namespace std;
using namespace std::chrono;

struct Clock
{
	uint32_t current_frame_cnt;
	uint32_t fps;
	steady_clock::time_point begin_time;
	steady_clock::time_point end_time;
	std::chrono::duration<double> time_diff;
	std::chrono::duration<double> fps_time_accum;

	void init()
	{
		current_frame_cnt = 0;
		fps_time_accum = fps_time_accum.zero();
		begin_time = steady_clock::now();
	}
	void tick()
	{
		end_time = steady_clock::now();
		time_diff = end_time - begin_time;
		begin_time = end_time;
		fps_time_accum += time_diff;
	}

	void calcFps()
	{
		//Every 100 frames calculate how long it took to come up with FPS average
		if (current_frame_cnt >= 100)
		{
			fps = current_frame_cnt / fps_time_accum.count();
			fps_time_accum *=0;
			current_frame_cnt = 0;
		}
	}


};

struct Config
{
public:
	float32 scaleFactor;

};



class Game {

public:
	Game() {}
	~Game() {}

	bool init();

	void render();
	void update();
	void handleEvents();
	void clean();
	void testBlocks(SDL_Event*, b2World*);

	bool running() { return bRunning; }
	int fps = 0;

	static Clock clock;
	static Util util;
	static TextureManager textureManager;
	static GameObjectManager gameObjectManager;
	static Config config;

	
private:
	//Main screen and window stuff
	SDL_Window* pWindow;
	string windowTitle, gameTitle, playerGameObjectId;
	int screenWidth, screenHeight;
	Uint32 windowXpos= SDL_WINDOWPOS_CENTERED, windowYPos= SDL_WINDOWPOS_CENTERED;
	Uint32 windowFlags= SDL_WINDOW_RESIZABLE;

	//Vector of all game objects
	vector<GameObject> gameObjects;
	GameObject* player;

	//Game States
	bool bRunning;

	//Box2d Physics
	b2Vec2 gravity;
	b2World* physicsWorld=nullptr;
	
	
	

	//Private Methods
	bool getConfig();

};



#endif

