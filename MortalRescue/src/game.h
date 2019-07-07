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
#include <array>

#include "Util.h"
#include "TextureManager.h"
#include "LevelManager.h"
#include "GameObjectManager.h"
#include "GameObjectContactListener.h"
#include "DynamicTextManager.h"
#include "Camera.h"
#include "DebugDraw.h"
#include "Settings.h"
#include "DebugPanel.h"

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
	bool debugPanel;
	SDL_Point debugPanelLocation;

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

	enum GameOjectLayer {

		BACKGROUND = 0,
		MAIN = 1,
		TEXT = 2,
		DEBUG = 3,
	};
	static const int MAX_LAYERS = 4;

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
	void addGameObject(GameObject* gameObject, int);
	void addGameObject(TextObject* gameObject, int);
	void addGameObject(DynamicTextObject* gameObject, int);
	void addGameObject(WorldObject* gameObject, int);
	bool getConfig();
	void buildWorld(string);

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
	DynamicTextManager dynamicTextManager;

	Config config;
	Camera camera;
	SDL_Rect worldBounds;
	SDL_Rect worldGridSize;
	b2World* physicsWorld;
	
	//Main screen and window stuff
	SDL_Window* pWindow;
	string gameTitle;
	Uint32 windowXpos= SDL_WINDOWPOS_CENTERED, windowYPos= SDL_WINDOWPOS_CENTERED;
	Uint32 windowFlags= SDL_WINDOW_RESIZABLE;
	float mouseSensitivity;

	//Array of Vectors of all game objects
	//Each array level is a layer containing a vector of all game obejcts for that layer
	array <vector <unique_ptr<GameObject>>, MAX_LAYERS> gameObjects;
	unique_ptr<PlayerObject> player;

	//Settings Object
	Settings settings;
	DebugPanel* debugPanel;

	//Box2d Physics
	b2Vec2 gravity;
	bool b2DebugDrawMode;
	float timeStep;
	int velocityIterations,
		positionIterations;


};





