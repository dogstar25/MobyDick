#include "Game.h"

int main(int argc, char* args[])
{
	Game::instance().init();

	while (Game::instance().gameState() != GameState::QUIT)
	{
		Game::instance().play();
	}

	return 0;
}