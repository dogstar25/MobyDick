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

