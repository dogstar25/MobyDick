#include <memory>

#include "Game.h"
#include "MRGame.h"
#include "MRContactListener.h"
#include "MRContactFilter.h"
#include "MRComponentFactory.h"

std::unique_ptr<Game> game;

int main(int argc, char* args[])
{
	game = std::make_unique<MRGame>();
	game->init(new MRContactListener(), new MRContactFilter(), new MRComponentFactory());

	while (game->gameState() != GameState::QUIT)
	{
		game->play();
	}

	return 0;
}