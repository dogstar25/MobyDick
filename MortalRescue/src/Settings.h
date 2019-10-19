#pragma once

#include <SDL.h>

#include <iostream>

using namespace std;

class Settings
{


public:

	void init();
	void run();
	void handleEvents();

	/*
	Settings Menu State
	*/
	enum State {

		EXITMENU = 0,
		RUN = 1,

	};

	int state;

	Settings();
	~Settings();
};

