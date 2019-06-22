#pragma once

#include <json/json.h>
#include <Box2D/Box2D.h>
#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <ctime>
#include <ratio>
#include <chrono>

#include "Util.h"
#include "TextureManager.h"
#include "LevelManager.h"
#include "GameObjectManager.h"
#include "GameObjectContactListener.h"
#include "Camera.h"
#include "DebugDraw.h"
#include "Settings.h"

//wow!
//#include <gl/GL.h>

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

struct Config
{
public:
	float32 scaleFactor;
	float32 mouseSensitivity;

};

//forward declations
class PlayerObject;
class GameObject;

using namespace std;
using namespace std::chrono;


class Game {

/*
Main Game State
*/


public:

	enum State {

		QUIT = 0,
		PLAY = 1,
		PAUSE = 2,
		SETTINGS = 3

	};

	Game();
	~Game();

	void play();
	void settingsMenu();

	bool init();
	void render();
	void update();
	void handleEvents();
	void buildLevel(string);
	void initWorldBounds();
	void addGameObject(GameObject* gameObject);
	void addGameObject(WorldObject* gameObject);
	bool getConfig();

	//Current Game State
	int gameState;

	int fps, awakeCount, gameLoopStep;
	string currentLevel;
	DebugDraw debugDraw;

	Clock clock;
	Util util;
	TextureManager textureManager;
	GameObjectManager gameObjectManager;
	LevelManager levelManager;
	GameObjectContactListener gameObjectContactListner;

	Config config;
	Camera camera;
	SDL_Rect worldBounds;
	b2World* physicsWorld;
	
	//Main screen and window stuff
	SDL_Window* pWindow;
	string gameTitle;
	Uint32 windowXpos= SDL_WINDOWPOS_CENTERED, windowYPos= SDL_WINDOWPOS_CENTERED;
	Uint32 windowFlags= SDL_WINDOW_RESIZABLE;
	float mouseSensitivity;

	//Vector of all game objects
	vector <unique_ptr<GameObject>> gameObjects;
	unique_ptr<PlayerObject> player;

	//Settings Object
	Settings settings;

	//Box2d Physics
	b2Vec2 gravity;
	bool debugDrawMode;
	float timeStep;
	int velocityIterations,
		positionIterations;


};





