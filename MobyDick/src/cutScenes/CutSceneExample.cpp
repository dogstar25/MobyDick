#include "CutSceneExample.h"

#include "../game.h"

extern std::unique_ptr<Game> game;



/*
Example cutscene
*/
void CutSceneExample::run(Scene* scene)
{
	currentScene = scene;

	if (currentState == cutSceneState::IDLE) {

		start();
	}
	else if (currentState == cutSceneState::RUNNING) {

		update();

	}

}

void CutSceneExample::start()
{

	currentState = cutSceneState::RUNNING;

	//_saveCurrentState();

	if (m_currentAct == 1) {

		//Get the player and disable the player controls
		_startPlayer();

		//Get Frank, give Frank a brain, and dispatch Frank to a destination
		_frank = _startFrank();
		_frank->dispatch({ 5400,900 });

		Camera::instance().setFollowMe(_frank);

		scene1Timer = Timer(15);

	}
	else if (m_currentAct == 2) {

		Camera::instance().dispatch({ 4000,900 });
 		
	}

}

void CutSceneExample::update()
{

	auto franksBrainState = _frank->brainState();

	if (m_currentAct == 1) {
		if (franksBrainState == BrainState::IDLE &&
			scene1Timer.hasMetTargetDuration() == true) {
			currentState = cutSceneState::IDLE;
			m_currentAct += 1;
		}
	}
	else if (m_currentAct == 2) {
		if (Camera::instance().state() == CameraState::IDLE) {
			currentState = cutSceneState::IDLE;
			m_currentAct += 1;
		}
	}

	if (m_currentAct > m_numberOfActs) {
		end();
	}

}

void CutSceneExample::end()
{
	SDL_Event event;

	//Call the wrapup functions for the pieces
	_endFrank();
	_endPlayer();

	//Create the event that will trigger the end of the scene to the sceneManager
	util::sendSceneEvent(SCENE_ACTION_RELEASE_DIRECT);

}

GameObject* CutSceneExample::_startPlayer()
{
	_player = currentScene->getGameObject("PlayerGina");
	_player->getComponent<PlayerControlComponent>(ComponentTypes::PLAYER_CONTROL_COMPONENT)->disable();

	return _player;
}

void CutSceneExample::_endPlayer()
{
	_player->getComponent<PlayerControlComponent>(ComponentTypes::PLAYER_CONTROL_COMPONENT)->enable();
	
}

GameObject* CutSceneExample::_startFrank()
{

	Json::Value componentsDefinition{};
	Json::Value brainDefinition{};

	GameObject* frank = currentScene->getGameObject("Frank");

	//Give Frank a brain
	brainDefinition["id"] = "BRAIN_COMPONENT";
	brainDefinition["sightSensorSize"] = 25;
	componentsDefinition["components"].append(brainDefinition);

	const auto& brainComponent =
		std::static_pointer_cast<BrainComponent>(
			game->componentFactory()->create(componentsDefinition, "Frank", "BOWMAN", currentScene, 0, 0, 0, ComponentTypes::BRAIN_COMPONENT)
			);
	brainComponent->setParent(frank);
	frank->addComponent(brainComponent);

	return frank;

}

void CutSceneExample::_endFrank()
{



}
