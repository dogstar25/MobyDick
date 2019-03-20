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
#include "GameObjectManager.h"
#include "GameObjectContactListener.h"
#include "Camera.h"
#include "DebugDraw.h"
#include "Settings.h"

//wow!
//#include <gl/GL.h>


//forward declations
struct Clock;
struct Config;
//class PlayerObject;
class LevelManager;
class GameObject;

using namespace std;
using namespace std::chrono;


class Game {

public:
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

	//Current Game State
	int gameState;

	int fps, awakeCount, gameLoopStep;
	string currentLevel;
	DebugDraw debugDraw;

	static Clock clock;
	static Util util;
	static TextureManager textureManager;
	static GameObjectManager gameObjectManager;
	static LevelManager levelManager;
	static Config config;
	static Camera camera;
	static SDL_Rect worldBounds;
	static b2World* physicsWorld;
	static vector <unique_ptr<GameObject>> gameObjects;

	/*
	Main Game State
	*/
	enum State {

		QUIT = 0,
		PLAY = 1,
		PAUSE = 2,
		SETTINGS = 3

	};

	
private:
	//Main screen and window stuff
	SDL_Window* pWindow;
	string gameTitle;
	Uint32 windowXpos= SDL_WINDOWPOS_CENTERED, windowYPos= SDL_WINDOWPOS_CENTERED;
	Uint32 windowFlags= SDL_WINDOW_RESIZABLE;
	float mouseSensitivity;

	//Vector of all game objects
	//vector<GameObject*> gameObjects;
	//GameObject* player;
	unique_ptr<GameObject> player;

	//Collision contact listener
	GameObjectContactListener gameObjectContactListner;

	//Settings Object
	Settings settings;

	//Box2d Physics
	b2Vec2 gravity;
	bool debugDrawMode;
	float timeStep;
	int velocityIterations,
		positionIterations;

	//Private Methods
	bool getConfig();

};

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


