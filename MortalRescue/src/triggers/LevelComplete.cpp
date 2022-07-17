#include "LevelComplete.h"

#include "SceneManager.h"
#include "../MRContextManager.h"
#include "game.h"

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

	util::sendSceneEvent(SCENE_ACTION_ADD, "SCENE_LEVEL_COMPLETE");

	m_hasTriggered = true;

	//Disable the player
	auto _player = SceneManager::instance().currentScene().getGameObject("PlayerGina");
	_player->getComponent<PlayerControlComponent>(ComponentTypes::PLAYER_CONTROL_COMPONENT)->disable();

	//Save the current level so we can continune from where we left off if we leave
	if (SceneManager::instance().currentScene().getNextLevel().has_value()) {

 		auto newLevelId = SceneManager::instance().currentScene().getNextLevel();

		auto saveFileData = std::make_shared<GameSaveFileData>();

		//First load whats currently on file
		game->contextMananger()->loadGame(saveFileData.get());

		std::istringstream(newLevelId.value()) >> saveFileData->level;

		game->contextMananger()->saveGame(saveFileData.get());

	}



}
