#include <stdio.h>
#include "Settings.h"

using namespace std;

void Settings::init()
{

}

void Settings::run()
{

	//build settings window
	this->state = RUN;
	cout << "Settings\n";

	while (this->state != EXITMENU)
	{

		handleEvents();

	}

}

void Settings::handleEvents()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{

		switch (event.type)
		{
		case SDL_KEYDOWN:
			if ((char)event.key.keysym.sym == SDLK_ESCAPE)
			{
				state = EXITMENU;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:

			//change the game menu state to exit and send the event
			state = EXITMENU;
			event.type = SDL_QUIT;
			SDL_PushEvent(&event);
			break;
		}
	}
}

Settings::Settings()
{
}


Settings::~Settings()
{
}
