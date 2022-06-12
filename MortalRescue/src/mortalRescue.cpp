#include <memory>

#include "Game.h"
#include "MRGame.h"
#include "MRContactListener.h"
#include "MRContactFilter.h"
#include "MRComponentFactory.h"
#include "triggers/MRTriggerFactory.h"
#include "cutScenes/MRCutSceneFactory.h"
#include "actions/MRActionFactory.h"
#include "IMGui/MR_IMGuiFactory.h"
#include "particleEffects/MRParticleEffectsFactory.h"
#include "MRContextManager.h"

std::unique_ptr<Game> game;

int main(int argc, char* args[])
{
	//create game 
	game = std::make_unique<MRGame>();

	//init game
	game->init(
		new MRContactListener(), 
		new MRContactFilter(),
		new MRComponentFactory(), 
		new MRActionFactory(), 
		new MRParticleEffectsFactory(),
		new MRCutSceneFactory(),
		new MR_IMGuiFactory(),
		new MRTriggerFactory(),
		new MRContextManager()
	);

	//play game
	game->play();

	return 0;
}