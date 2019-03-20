// MortalRescue.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdlib.h>
#include <crtdbg.h>


#include "game.h"
#include <math.h>

Game* game = 0;
using namespace std;

int main(int argc, char *args[])

{
	
	cout << "Mortal Rescue Begins\n";

	game = new Game();
	game->init();

	while (game->gameState != game->QUIT) 
	{

		game->handleEvents();

		switch (game->gameState)
		{
			case game->PAUSE:
			case game->SETTINGS:
				game->settingsMenu();
				break;

			case game->PLAY:
				game->play();
				break;
		}
	}

	delete game;

	return 0;

}


