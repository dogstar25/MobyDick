// MortalRescue.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "game.h"

Game* game = 0;

int main(int argc, char *args[])

{
    std::cout << "Mortal Rescue Begins\n"; 

	game = new Game();
	game->init();

	while (game->running()) {
		game->handleEvents();
		game->update();
		game->render();
	}

	game->clean();

	return 0;

}


