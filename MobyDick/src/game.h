#pragma once

#include <json/json.h>
#include <Box2D/Box2D.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>


#include <iostream>
#include <fstream>
//#include <cstdio>
#include <ctime>
#include <ratio>
#include <chrono>
#include <array>
#include <forward_list>
#include <string>

#include "Globals.h"
#include "GameObjectManager.h"
#include "Util.h"
#include "Constants.h"
#include "TextureManager.h"
#include "Level.h"

#include "GameObjectContactListener.h"
#include "DynamicTextManager.h"
#include "ObjectPoolManager.h"
#include "Camera.h"
#include "DebugDraw.h"
#include "DebugPanel.h"
#include "GameObjectCollection.h"
#include "ParticleMachine.h"




//forward declations
class PlayerObject;
class GameObject;
class ParticleObject;
class CompositeObject;
class ParticleMachine;
class WeaponObject;

/*
	Main Game Class
*/
class Game {

private:
	GameObjectContactListener m_gameObjectContactListner;

public:

	Game();
	~Game();

	void play();
	bool init();
	void render();
	static void renderCollection(array<GameObjectCollection, constants::MAX_GAMEOBJECT_LAYERS>*);
	void update();
	void handleEvents();
	
	void addGameObject(GameObject* gameObject, int);
	void addGameObject(TextObject* gameObject, int);
	void addGameObject(WorldObject* gameObject, int);
	void addGameObject(ParticleObject* gameObject, int);
	void addGameObject(CompositeObject* gameObject, int);
	void addGameObject(WeaponObject* gameObject, int);
	void buildWorld(string);

	b2World* physicsWorld;
	
	//Main screen and window stuff
	SDL_Window* pWindow;
	int gameObjectCount;

	//mouse
	float mouseSensitivity;
	b2Vec2 mouseLocation, mouseClickLocation;

	//Fixed array of Layers
	//Each layer contains a GameObjectCollection
	array <GameObjectCollection, constants::MAX_GAMEOBJECT_LAYERS> gameCollections;
	PlayerObject* player;

	unique_ptr<DebugPanel> debugPanel;

	//Current Game State
	int gameState;

	int fps;
	DebugDraw debugDraw;


	//Accessor Functions



};





