#include "Game.h"

Game* game;
using namespace std;

int main(int argc, char* args[])
{

	cout << "Mortal Rescue Begins!\n";

	game = new Game();
	game->init();

	while (game->gameState != GameState::QUIT)
	{

		game->handleEvents();

		switch (game->gameState)
		{
		case GameState::PAUSE:
			//this may be not needed
			break;

		case GameState::PLAY:
			game->play();
			break;
		}
	}

	delete game;

	return 0;
}