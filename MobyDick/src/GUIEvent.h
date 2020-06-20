#pragma once
#include <SDL2/SDL.h>
#include "Game.h"
#include "GameObject.h"
#include "GameObjectCollection.h"

#include <iostream>
#include <array>

using namespace std;


class GUIEvent
{

public:
	GUIEvent(string guiObjectId);
	~GUIEvent();

	void run();
	void handleInput();
	void update();
	void render();
	

	/*
	Settings Menu State
	*/
	enum State {

		EXITGUI = 0,
		ACTIVE = 1,

	};

	int state;
	array <GameObjectCollection, constants::MAX_GAMEOBJECT_LAYERS> uiObjectCollections;

};

