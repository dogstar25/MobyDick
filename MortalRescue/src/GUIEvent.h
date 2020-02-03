#pragma once
#include <SDL.h>
#include "game.h"
#include "GameObject.h"
#include "GameObjectCollection.h"

#include <iostream>
#include <array>

using namespace std;


class GUIEvent
{

public:
	GUIEvent();
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
	array <GameObjectCollection, Game::MAX_LAYERS> uiObjectCollections;

};

