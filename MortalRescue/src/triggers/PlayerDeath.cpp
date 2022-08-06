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
		if (objectiveStatusItem.value() <= 0) {
			hasMet = true;
		}

	}

	return hasMet;
}


void PlayerDeath::execute()
{


	util::sendSceneEvent(SCENE_ACTION_ADD, "SCENE_PLAYER_DEATH");

	m_hasTriggered = true;

	auto playerObject = SceneManager::instance().currentScene().getGameObject("PlayerGina");
	assert(playerObject.has_value() && "GameObject wasnt found!");

	auto _player = playerObject.value();
	_player->getComponent<PlayerControlComponent>(ComponentTypes::PLAYER_CONTROL_COMPONENT)->disable();




}
