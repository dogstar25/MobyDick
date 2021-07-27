#include <memory>

#include "Game.h"
#include "StubGame.h"
#include "ContactListener.h"
#include "ContactFilter.h"
#include "ComponentFactory.h"

std::unique_ptr<Game> game;

int main(int argc, char* args[])
{
	game = std::make_unique<StubGame>();
	game->init(new ContactListener(), new ContactFilter(), new ComponentFactory());

	while (game->gameState() != GameState::QUIT)
	{
		game->play();
	}

	return 0;
}