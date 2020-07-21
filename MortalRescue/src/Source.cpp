#include "Game.h"

#include <iostream>
#include <SDL2/SDL.h>


int main(int argc, char* args[])
{

	std::cout << "Mortal Rescue Begins\n";

	Game::instance().init();

	while (Game::instance().gameState() != GameState::QUIT)
	{

		switch (Game::instance().gameState())
		{
		case GameState::PAUSE:
			//this may be not needed
			break;

		case GameState::PLAY:
			Game::instance().play();
			break;
		}
	}
	return 0;
}