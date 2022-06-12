#include "LevelComplete.h"

#include "../SceneManager.h"
#include "../game.h"

extern std::unique_ptr<Game> game;


LevelComplete::LevelComplete():
	Trigger()
{
	m_triggerOnlyOnce = true;

}

bool LevelComplete::hasMetCriteria(Scene* scene)
{

	bool hasMet{ false };

	if (m_triggerOnlyOnce == false || ( m_triggerOnlyOnce && m_hasTriggered == false)) {

		//Loop through all of the objectives and see if they have been met
		for (const auto& objective : scene->objectives()) {

			hasMet = true;

			//Get the value that ties back to this objective from the contextManager
			auto objectiveStatusValue = game->contextMananger()->getStatusItemValue(objective.contextManagerId);

			if (objectiveStatusValue < objective.targetValue) {

				hasMet = false;
				break;
			}
		}
	}

	return hasMet;
}


void LevelComplete::execute()
{

	SDL_Event event;

	SceneAction* sceneAction = new SceneAction();
	sceneAction->actionCode = SCENE_ACTION_ADD;
	sceneAction->actionId = "SCENE_LEVEL_COMPLETE";

	event.type = SDL_USEREVENT;
	event.user.data1 = sceneAction;
	SDL_PushEvent(&event);

	m_hasTriggered = true;



}
