#include "SceneManager.h"

#include <fstream>

#include "Clock.h"
#include "DynamicTextManager.h"
#include "DebugPanel.h"


SceneManager::~SceneManager()
{

}

SceneManager& SceneManager::instance()
{

	static SceneManager singletonInstance;
	return singletonInstance;

}

void SceneManager::init()
{
	m_currentSceneIndex = 0;
	m_scenes.reserve(MAX_SCENES);

	float gameLoopStep = GameConfig::instance().gameLoopStep();
	m_gameTimer = Timer(gameLoopStep);

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
		std::string gameObjectId = itr["id"].asString();


		auto maxObjects = itr["maxObjects"].asInt();

		//Json::Value sceneJSON = Json::Value(itr);
		m_sceneDefinitions.emplace(gameObjectId, Json::Value(itr));
	}
}


void SceneManager::run()
{

	//Only update and render if we have passed the 60 fps time passage
	if (m_gameTimer.hasMetTargetDuration() == true)
	{
		//Calculate and display the FPS
		m_frameCount++;
		float fps = m_gameTimer.calculateFPS(m_frameCount);
		if (fps > 0) {
			DynamicTextManager::instance().updateText("FPS_VALUE", util::floatToString(fps, 2));
		}

		//Run update for every active scene
		for (auto& scene : m_scenes) {
			if (scene.state() == SceneState::RUN) {
				scene.update();
			}
		}

		//Clear the screen
		Renderer::instance().clear();

		//Render every scene, active or not
		for (auto& scene : m_scenes) {
			scene.render();
		}

		//Display all rendered objects
		Renderer::instance().present();
	}

}

std::optional<SceneAction> SceneManager::pollEvents()
{
	int keyCode, keyStateCount;
	SDL_Event event;
	//PlayerInputEvent* playerInputEvent = nullptr;
	const Uint8* keyStates = nullptr;
	std::optional<SceneAction> sceneAction;

	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	//Handle special events and everything else should be player control
	//input related so staore it for later
	while (SDL_PollEvent(&event) && sceneAction.has_value() == false)
	{
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
				//if (type == SCENE_ACTION_TYPE) {
				//	sceneAction = *(static_cast<std::optional<SceneAction>*>(event.user.data1));
				//}
				//else if (type == LEVEL_ACTION_TYPE) {

				//	levelAction = *(static_cast<std::optional<LevelAction>*>(event.user.data1));
				//}
				//std::optional<SceneActionCode*> actionCode = (SceneActionCode*)event.user.data1;
				sceneAction = *(static_cast<std::optional<SceneAction>*>(event.user.data1));
				delete event.user.data1;
				break;

				//TODO:
				//Check what the action code value in this void ptr is and if its a level action then
				//put it in th levelManager
				//Like player killed, level completed, etc - this , in turn, could trigger creation of a sceneAction



			}
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEMOTION:
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
	m_scenes.back().setState(SceneState::RUN);

}

Scene& SceneManager::pushScene(std::string sceneId)
{
	if (m_scenes.empty() == false) {
		m_scenes.back().setState(SceneState::PAUSE);
	}
	Scene& scene = SceneManager::instance().scenes().emplace_back(sceneId);

	return scene;
	
}

GameObject* SceneManager::addGameObject(std::shared_ptr<GameObject>gameObject, int layer)
{
	//Add the gameObject to the currently active scene using back()
	auto gameObjectRef = m_scenes.back().addGameObject(gameObject, layer);

	return gameObjectRef;

}

GameObject* SceneManager::addGameObject(std::string gameObjectId, int layer, float xMapPos, float yMapPos, float angle, bool cameraFollow)
{
	//Add the gameObject to the currently active scene using back()
	auto& currentScene = m_scenes.back();
	auto gameObject = currentScene.addGameObject(gameObjectId, layer, xMapPos, yMapPos, angle, cameraFollow);

	return gameObject;

}

