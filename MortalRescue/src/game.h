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
using namespace std;

struct Clock
{
	uint32_t last_tick_time = 0;
	uint32_t delta = 0;

	void tick()
	{
		uint32_t tick_time = SDL_GetTicks();
		delta = tick_time - last_tick_time;
		last_tick_time = tick_time;
	}
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

	bool running() { return m_bRunning; }
	static Clock clock;

	

private:
	//Game Window Settings
	//string windowTitle;
	string windowTitle, gameTitle, playerGameObjectId;
	int screenWidth, screenHeight;
	Uint32 windowXpos= SDL_WINDOWPOS_CENTERED, windowYPos= SDL_WINDOWPOS_CENTERED;
	Uint32 windowFlags= SDL_WINDOW_RESIZABLE;

	//Manager objects
	TextureManager textureManager;
	GameObjectManager gameObjectManager;
	

	vector<GameObject> gameObjects;
	GameObject* player;
		
	SDL_Window* pWindow;
	bool m_bRunning;

	bool getConfig();
};

#endif

