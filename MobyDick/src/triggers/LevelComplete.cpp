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
	bool increasingTarget{};

	if (m_triggerOnlyOnce == false || ( m_triggerOnlyOnce && m_hasTriggered == false)) {

		bool failedObjectiveFound{ false };

		//Loop through all of the objectives and see if they have been met
		for (const auto& objective : scene->objectives()) {

			//Get the value that ties back to this objective from the contextManager
			auto objectiveStatusItem = game->contextMananger()->getStatusItem(objective.id);

			//Determine if we need to check for < or > to see if we have met criteria
			auto objectiveStatusInitialValue = objectiveStatusItem.initialvalue();
			if (objectiveStatusInitialValue < objective.targetValue) {
				increasingTarget = true;
			}
			else {
				increasingTarget = false;
			}

			auto objectiveStatusValue = objectiveStatusItem.value();

			if (increasingTarget == true && objectiveStatusValue >= objective.targetValue) {

				//objective was met
				continue;
			}
			else if (increasingTarget == false && objectiveStatusValue <= objective.targetValue) {

				//objective was met
				continue;
			}
			else {
				//objective was not met
				failedObjectiveFound = true;
				break;
			}
		}

		if (failedObjectiveFound == false) {
			hasMet = true;
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

	auto _player = SceneManager::instance().currentScene().getGameObject("PlayerGina");
	_player->getComponent<PlayerControlComponent>(ComponentTypes::PLAYER_CONTROL_COMPONENT)->disable();




}
