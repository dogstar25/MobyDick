#include "EnemyCDeadTrigger.h"

#include "SceneManager.h"
#include "game.h"
#include "../GameConstants.h"
#include "triggers/Trigger.h"
#include "../particleEffects/GameParticleEffects.h"
#include "SoundManager.h"


extern std::unique_ptr<Game> game;

EnemyCDeadTrigger::EnemyCDeadTrigger() :
	Trigger()
{
	m_triggerOnlyOnce = true;
	m_triggerName = "EnenmyCDeadTrigger";
	m_criteriaCheckTimer = Timer(1, true);

}

bool EnemyCDeadTrigger::hasMetCriteria(Scene* scene)
{

	bool hasMet{ false };

	//Call base first
	if(Trigger::hasMetCriteria(scene)){

		std::cout << "checked" << std::endl;
		if (m_triggerOnlyOnce == false || (m_triggerOnlyOnce && m_hasTriggered == false)) {

			const auto& enemyC = scene->getGameObjectsByName("EnemyC");
			if (enemyC.empty()) {
				hasMet = true;
			}

		}
	}

	return hasMet;
}

void EnemyCDeadTrigger::execute()
{

	auto triggerTargetObjects = SceneManager::instance().currentScene().getGameObjectsByName("EnemyCTrigger_TARGET");

	m_hasTriggered = true;

	//Toggle the disabled flags
	for (auto& targetObject : triggerTargetObjects) {

		if (targetObject->renderDisabled() == true && targetObject->physicsDisabled() == true) {
			targetObject->enableRender();
			targetObject->enablePhysics();

		}
		else if (targetObject->updateDisabled() == true) {
			targetObject->enableUpdate();
		}
		else {
			targetObject->disableRender();
			targetObject->disablePhysics();
		}

		//If the object being toggled has a toggle sound then play it
		if (targetObject->hasComponent(ComponentTypes::SOUND_COMPONENT) == true) {

			const auto& soundComponent = targetObject->getComponent<SoundComponent>(ComponentTypes::SOUND_COMPONENT);
			soundComponent->playSound("TOGGLE_SOUND");
		}

	}


}

