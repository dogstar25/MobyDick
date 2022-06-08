#include "LevelComplete.h"

#include "../SceneManager.h"
#include "../game.h"

extern std::unique_ptr<Game> game;

bool LevelComplete::hasMetCriteria()
{

	bool hasMet{ false };


	//Loop through all of the objectives and see if they have been met
	for (const auto& objective : LevelManager::instance().objectives()) {

		hasMet = true;

		//Get the value that ties back to this objective from the contextManager
		auto objectiveStatusValue = game->contextMananger()->getStatusItemValue(objective.contextManagerId);

		if (objectiveStatusValue < objective.targetValue) {

			hasMet = false;
			break;
		}
	}

	return hasMet;
}


void LevelComplete::execute()
{

	/*SDL_Event event;

	SceneAction* sceneAction = new SceneAction();
	sceneAction->actionCode = SCENE_ACTION_ADD;
	sceneAction->sceneId = "SCENE_PLAYER_DEATH";

	event.type = SDL_USEREVENT;
	event.user.data1 = sceneAction;
	SDL_PushEvent(&event);*/


	Scene& scene = SceneManager::instance().scenes().back();
	scene.loadLevel("level2");



	//instead of this how about adding a "GUI_PLAYERDEATH_PANEL" gameObject to the current scene


}
