// MortalRescue.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "game.h"

Game* game;
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
		case Game::PAUSE:
			//this may be not needed
				break;

		case Game::PLAY:
				game->play();
				break;
		}
	}

	delete game;

	return 0;

}


