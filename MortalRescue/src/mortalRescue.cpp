#include <memory>

#include "Game.h"
#include "MRGame.h"
#include "MRContactListener.h"
#include "MRContactFilter.h"
#include "MRComponentFactory.h"
#include "actions/MRActionFactory.h"
#include "particleEffects/MRParticleEffectsFactory.h"
#include "MRStatusManager.h"

std::unique_ptr<Game> game;

int main(int argc, char* args[])
{
	game = std::make_unique<MRGame>();

	game->init(
		new MRContactListener(), 
		new MRContactFilter(),
		new MRComponentFactory(), 
		new MRActionFactory(), 
		new MRParticleEffectsFactory, 
		new MRStatusManager()
	);

	while (game->gameState() != GameState::QUIT)
	{
		game->play();
	}

	return 0;
}