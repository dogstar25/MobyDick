#include "CutSceneDiscoverTreasure.h"

#include "components/BrainComponent.h"
#include "Camera.h"
#include "game.h"

extern std::unique_ptr<Game> game;



/*
Example cutscene
*/
void CutSceneDiscoverTreasure::run(Scene* scene)
{
	currentScene = scene;

	if (currentState == cutSceneState::IDLE) {

		start();
	}
	else if (currentState == cutSceneState::RUNNING) {

		update();

	}

}

void CutSceneDiscoverTreasure::start()
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

		_frank->dispatch({ 1,1 });
		Camera::instance().dispatch({ 4000,900 });
	}

}

void CutSceneDiscoverTreasure::update()
{

	if (m_currentAct == 1) {
		if (_frank->brainState() == BrainState::IDLE &&
			scene1Timer.hasMetTargetDuration() == true) {
			currentState = cutSceneState::IDLE;
			m_currentAct += 1;
		}
	}
	else if (m_currentAct == 2) {
		if (_frank->brainState() == BrainState::IDLE &&
			Camera::instance().state() == CameraState::IDLE) {
			currentState = cutSceneState::IDLE;
			m_currentAct += 1;
		}
	}

	if (m_currentAct > m_numberOfActs) {
		end();
	}

}

void CutSceneDiscoverTreasure::end()
{
	SDL_Event event;

	//Call the wrapup functions for the pieces
	_endFrank();
	_endPlayer();

	//Create the event that will trigger the end of the scene to the sceneManager
	SceneAction* sceneAction = new SceneAction();
	sceneAction->actionCode = SCENE_ACTION_RELEASE_DIRECT;
	sceneAction->actionId = "";
	event.type = SDL_USEREVENT;
	event.user.data1 = sceneAction;
	SDL_PushEvent(&event);

}

GameObject* CutSceneDiscoverTreasure::_startPlayer()
{
	_player = currentScene->getGameObject("PlayerGina");
	_player->getComponent<PlayerControlComponent>(ComponentTypes::PLAYER_CONTROL_COMPONENT)->disable();

	return _player;
}

void CutSceneDiscoverTreasure::_endPlayer()
{
	_player->getComponent<PlayerControlComponent>(ComponentTypes::PLAYER_CONTROL_COMPONENT)->enable();
	
}

GameObject* CutSceneDiscoverTreasure::_startFrank()
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
			game->componentFactory()->create(componentsDefinition, "SURVIVOR", currentScene, 0, 0, 0, ComponentTypes::BRAIN_COMPONENT)
			);
	brainComponent->setParent(frank);
	frank->addComponent(brainComponent, ComponentTypes::BRAIN_COMPONENT);

	return frank;

}

void CutSceneDiscoverTreasure::_endFrank()
{



}
