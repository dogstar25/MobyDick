#ifndef GUIEVENT_H
#define GUIEVENT_H

#include <array>
#include <string>

#include "Constants.h"
#include "GameObjectCollection.h"


class GUIEvent
{

public:
	GUIEvent(std::string guiObjectId);
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
	std::array <GameObjectCollection, constants::MAX_GAMEOBJECT_LAYERS> uiObjectCollections;

};

#endif