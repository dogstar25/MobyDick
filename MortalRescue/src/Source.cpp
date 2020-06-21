#include "Game.h"

using namespace std;

int main(int argc, char* args[])
{

	cout << "Mortal Rescue Begins\n";

	Game::instance().init();

	while (Game::instance().gameState != GameState::QUIT)
	{

		Game::instance().handleEvents();

		switch (Game::instance().gameState)
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