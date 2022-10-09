#include "PlayerDeath.h"

#include "SceneManager.h"
#include "game.h"
#include "../GameConstants.h"
#include "triggers/Trigger.h"
#include "../particleEffects/GameParticleEffects.h"
#include "SoundManager.h"


extern std::unique_ptr<Game> game;

PlayerDeath::PlayerDeath() :
	Trigger()
{
	m_triggerOnlyOnce = true;
	m_triggerName = "PlayerDeath";

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

	//If this trigger can trigger again then reset the criteria
	if (m_triggerOnlyOnce == false) {

		reset();
	}



}

void PlayerDeath::reset()
{

	m_hasTriggered = false;
	game->contextMananger()->getStatusItem(StatusItemId::PLAYERS_HEART_COUNT).reset();

}