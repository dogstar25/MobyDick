#include "PlayerDeath.h"

#include "SceneManager.h"
#include "game.h"
#include "../GameConstants.h"
#include "triggers/Trigger.h"

extern std::unique_ptr<Game> game;

PlayerDeath::PlayerDeath() :
	Trigger()
{
	m_triggerOnlyOnce = true;

}

bool PlayerDeath::hasMetCriteria(Scene* scene)
{

	bool hasMet{ false };

	if (m_triggerOnlyOnce == false || (m_triggerOnlyOnce && m_hasTriggered == false)) {


		auto objectiveStatusItem = game->contextMananger()->getStatusItem(StatusItemId::PLAYERS_HEART_COUNT);
		if (objectiveStatusItem.value() == 0) {
			hasMet = true;
		}

	}

	return hasMet;
}


void PlayerDeath::execute()
{

	SDL_Event event;

	SceneAction* sceneAction = new SceneAction();
	sceneAction->actionCode = SCENE_ACTION_ADD;
	sceneAction->actionId = "SCENE_PLAYER_DEATH";

	event.type = SDL_USEREVENT;
	event.user.data1 = sceneAction;
	SDL_PushEvent(&event);

	m_hasTriggered = true;

	auto _player = SceneManager::instance().currentScene().getGameObject("PlayerGina");
	_player->getComponent<PlayerControlComponent>(ComponentTypes::PLAYER_CONTROL_COMPONENT)->disable();




}
