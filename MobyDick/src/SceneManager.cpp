#include "SceneManager.h"

#include <fstream>

#include "game.h"
#include "IMGui/IMGuiUtil.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdlrenderer.h"



extern std::unique_ptr<Game> game;

SceneManager::~SceneManager()
{

}

SceneManager& SceneManager::instance()
{

	static SceneManager singletonInstance;
	return singletonInstance;

}

void SceneManager::deleteGameObject(std::string gameObjectName)
{
	auto foundGameObjects = m_scenes.back().getGameObjectsByName(gameObjectName);
	for (auto gameObject : foundGameObjects) {
		gameObject->setRemoveFromWorld(true);
	}

}



void SceneManager::init()
{
	m_currentSceneIndex = 0;
	m_scenes.reserve(MAX_SCENES);

	float gameLoopStep = GameConfig::instance().gameLoopStep();
	m_gameTimer = Timer(gameLoopStep, true);

}

Scene& SceneManager::getScene(std::string sceneId)
{

	for (auto scene : m_scenes) {
		if (scene.id() == sceneId) {
			return scene;
		}
	}

}

void SceneManager::load(std::string sceneDefinitionsFilename)
{

	//Read file and stream it to a JSON object
	Json::Value root;
	std::string filename = "assets/config/" + sceneDefinitionsFilename + ".json";
	std::ifstream ifs(filename);
	ifs >> root;

	for (Json::Value itr : root["scenes"])
	{
		std::string sceneId = itr["id"].asString();


		auto maxObjects = itr["maxObjects"].asInt();

		//Json::Value sceneJSON = Json::Value(itr);
		m_sceneDefinitions.emplace(sceneId, Json::Value(itr));
	}
}


void SceneManager::run()
{

	//Only update and render if we have passed the 60 fps time passage
	if (m_gameTimer.hasMetTargetDuration() == true)
	{
		//Start a new IMGui Frame that will allow for any new windows to be created throughout 
		//the main loop
		ImGui::MobyDickNewFrame();

		//IMGui framerate calc - R.I.P MobyDick Framerate code 2018 - 2022
		ImGui::MobyDickFPSFrame();

		//Run update for every active scene
		for (auto& scene : m_scenes) {

			if (scene.state() == SceneState::RUN) {
				scene.update();
			}
		}

		//Clear the screen
		game->renderer()->clear();

		//Render every scene, active or not
		for (auto& scene : m_scenes) {
			scene.render();
		}

		//Render any IMGui frames that were updated in this loop
		ImGui::MobyDickRenderFrame();

		//Display all rendered objects
		game->renderer()->present();

	}



}

std::optional<SceneAction> SceneManager::pollEvents()
{
	int keyCode, keyStateCount;
	SDL_Event event;
	//PlayerInputEvent* playerInputEvent = nullptr;
	const Uint8* keyStates = nullptr;
	std::optional<SceneAction> sceneAction;
	sceneAction = std::nullopt;

	//Poll events and as soon as we find a sceneReloated action, break and handle it in game.cpp
	//The "sceneAction.has_value()" check HAS to happen first in the below while, otherwise, an event could be lost during 
	// the SDL_PollEvent() call
	while (sceneAction.has_value() == false && SDL_PollEvent(&event) )
	{

		//Pass this event along to IMGUI so that all IMGUI windows can detect mouse and keyboard events
		ImGui_ImplSDL2_ProcessEvent(&event);

		switch (event.type)
		{

			case SDL_KEYUP:
			case SDL_KEYDOWN:
			{

				keyCode = event.key.keysym.sym;

				/*
				Translate the keycode to whatever action it is tied to in this scene
				Only interested in KEYDOWN for sceneActions
				*/
				if (event.type == SDL_KEYDOWN) {
					sceneAction = getSceneKeyAction(keyCode);
				}

				/*
				If no mapping was found for the keycode, then assume that
				it is player action related and store it for later
				*/
				if (sceneAction.has_value() == false) {
					//std::cout << "\033[1;31m Store Key\033[0m" << keyCode << "\n";
					PlayerInputEvent& playerInputEvent = m_PlayerInputEvents.emplace_back();
					playerInputEvent.event = event;

					//Get the keyboard state array and copy it to our save spot - memcpy!!!
					keyStates = SDL_GetKeyboardState(&keyStateCount);
					memcpy_s(playerInputEvent.keyStates,
						SDL_NUM_SCANCODES * sizeof(unsigned char),
						keyStates,
						keyStateCount * sizeof(unsigned char));
				}

				break;
			}
			case SDL_USEREVENT:
			{

				
				int* type = static_cast<int*>(event.user.data1);
				sceneAction = *(static_cast<std::optional<SceneAction>*>(event.user.data1));
				delete event.user.data1;

				break;

			}
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN:
			{
				PlayerInputEvent& playerInputEvent = m_PlayerInputEvents.emplace_back();
				playerInputEvent.event = event;

				break;
			}
			default:
				break;

		}

	}

	return sceneAction;

}

std::optional<SceneAction> SceneManager::getSceneKeyAction(SDL_Keycode keycode)
{
	return m_scenes.back().getkeycodeAction(keycode);
}

void SceneManager::popScene()
{

	m_scenes.pop_back();
	if (!m_scenes.empty()) {
		_restoreSceneState(m_scenes.back().id());
	}

}

Scene& SceneManager::pushScene(std::string sceneId, bool pausePreviousScene)
{
	if (m_scenes.empty() == false) {

		//Save the currect state and position of ket objects
		_saveCurrentState(m_scenes.back().id());

		//Pause the current Scene if directed
		if (pausePreviousScene) {
			m_scenes.back().setState(SceneState::PAUSE);
		}
	}
	Scene& scene = SceneManager::instance().scenes().emplace_back(sceneId);

	return scene;
	
}

void SceneManager::loadLevel(std::string levelId)
{
	m_scenes.back().loadLevel(levelId);
}

void SceneManager::loadNextLevel()
{
	m_scenes.back().loadNextLevel();
}

void SceneManager::loadCurrentLevel()
{
	m_scenes.back().loadCurrentLevel();
}

void SceneManager::directScene(std::string cutSceneId)
{

	//if wer are already in the middle of a curscene then disregard the
	// new cutscene trying to be added
	if (m_scenes.back().cutScene().has_value() == false) {
		//disable and enable certain game objects and stuff
		_directorTakeOver();

		std::shared_ptr<CutScene> cutScene = game->cutSceneFactory()->create(cutSceneId);
		//std::shared_ptr<CutScene> cutScene = std::make_shared<CutSceneExample>();
		m_scenes.back().setCutScene(cutScene);
	}

}

void SceneManager::releaseDirectScene()
{
	_directorRelease();
	m_scenes.back().deleteCutScene();

}

void SceneManager::respawnPlayer()
{

	int highestSequence = 0;

	//auto playerOrig = m_scenes.back().playerOriginalSpawnPoint();
	std::optional<std::shared_ptr<CheckPointComponent>> highestCheckpoint{};

	//loop through all gameObjects and find the checkPoint object with the highest sequence
	for (const auto& gameObject : m_scenes.back().gameObjects()[GameLayer::ABSTRACT]) {

		if (gameObject->hasComponent(ComponentTypes::CHECKPOINT_COMPONENT)) {

			const std::shared_ptr<CheckPointComponent> checkpointComponent = 
				gameObject->getComponent<CheckPointComponent>(ComponentTypes::CHECKPOINT_COMPONENT);

			if (checkpointComponent->hasMetCriteria() && highestSequence < checkpointComponent->sequence()) {

				highestSequence = checkpointComponent->sequence();
				highestCheckpoint = checkpointComponent;
			}

		}
	}

	//Get the player gameObject
	auto playerObject = m_scenes.back().getGameObjectsByTrait(TraitTag::player);
	assert(!playerObject.empty() && "GameObject wasnt found!");
	//There should only be one player
	auto player = playerObject[0];

	//Determine spawn position. Either the furthest checkpoint or the original spawn location
	SDL_FPoint spawnLocation{};
	if (highestCheckpoint.has_value()) {

		spawnLocation = util::tileToPixelLocation(
			highestCheckpoint->get()->playerSpawnLocation().x,
			highestCheckpoint->get()->playerSpawnLocation().y,
			player->getSize().x,
			player->getSize().y);
	}
	else {

		spawnLocation = player->getOriginalPosition();

	}

	player->enableCollision();
	player->enableRender();

	player->setPosition(spawnLocation);

	//Restore Health
	auto vitality = player->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);
	vitality->setHealth(6);

	//Restore Player Control
	auto playerControl = player->getComponent<PlayerControlComponent>(ComponentTypes::PLAYER_CONTROL_COMPONENT);
	playerControl->enable();

	//Reset the playerDeath Trigger
	m_scenes.back().resetTrigger("PlayerDeath");



}

GameObject* SceneManager::addGameObject(std::shared_ptr<GameObject>gameObject, int layer)
{
	//Add the gameObject to the currently active scene using back()
	m_scenes.back().addGameObject(gameObject, layer);

	//ToDo:fix this warning
	return gameObject.get();

}

GameObject* SceneManager::addGameObject(std::string gameObjectType, int layer, float xMapPos, float yMapPos, float angle, bool cameraFollow)
{
	//Add the gameObject to the currently active scene using back()
	auto& currentScene = m_scenes.back();
	auto gameObject = currentScene.addGameObject(gameObjectType, layer, xMapPos, yMapPos, angle, cameraFollow);

	return gameObject;

}

void SceneManager::_directorTakeOver()
{

	_saveCurrentState("DIRECTED");

}

void SceneManager::_directorRelease()
{

	_restoreSceneState("DIRECTED");

}

void SceneManager::_saveCurrentState(std::string sceneId)
{
	SceneSnapshot snapshot{};
	snapshot.state = m_scenes.back().state();
	snapshot.inputControlMode = m_scenes.back().inputControlMode();
	snapshot.cameraFollowedObject = Camera::instance().getFollowMeObject();
	snapshot.cameraLocation = { Camera::instance().frame().x, Camera::instance().frame().y };

	m_sceneSaveSnapshots[sceneId] = snapshot;

}

void SceneManager::_restoreSceneState(std::string sceneId)
{

	assert(m_sceneSaveSnapshots.find(sceneId) != m_sceneSaveSnapshots.end() && "SceneId not found in saved scene snapshots map");

	m_scenes.back().setState(m_sceneSaveSnapshots[sceneId].state);
	m_scenes.back().setInputControlMode(m_sceneSaveSnapshots[sceneId].inputControlMode);
	if (m_sceneSaveSnapshots[sceneId].cameraFollowedObject != nullptr) {
		Camera::instance().setFollowMe(m_sceneSaveSnapshots[sceneId].cameraFollowedObject);
	}
	else {
		Camera::instance().setFramePosition(m_sceneSaveSnapshots[sceneId].cameraLocation.x, m_sceneSaveSnapshots[sceneId].cameraLocation.y);
	}

	auto it = m_sceneSaveSnapshots.find(sceneId);
	m_sceneSaveSnapshots.erase(it);

}

void SceneManager::testIMGUI()
{

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	//ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	bool show_demo_window = true;
	ImGui::ShowDemoWindow(&show_demo_window);

	bool show_another_window = true;
	ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	ImGui::Text("Hello from another window!");
	if (ImGui::Button("Close Me"))
		show_another_window = false;
	ImGui::End();

	// Rendering
	ImGui::Render();
	//glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	//glClearColor(0.0, 0.0, 0.0, 0.0);
	//glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	//SDL_GL_SwapWindow(window);

}
