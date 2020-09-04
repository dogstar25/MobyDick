#pragma once
#include <vector>
#include <memory>
#include <optional>
#include <functional>

#include <json/json.h>

#include "Scene.h"
#include "Globals.h"

struct PlayerInputEvent
{

	unsigned char keyStates[SDL_NUM_SCANCODES];
	SDL_Event event;
};


class SceneManager
{

public:
	SceneManager();
	~SceneManager();

	static SceneManager& instance();
	void init();
	void load(std::string sceneDefinitionsFilename);
	void run();
	std::optional<SceneAction> pollEvents();
	void popScene();
	Scene& pushScene(std::string sceneId);

	std::optional<SceneAction> getSceneKeyAction(SDL_Keycode);

	auto currentSceneIndex(){
		return m_currentSceneIndex;
	}
	auto setCurrentSceneIndex( int newCurrentSceneIndex) {
		m_currentSceneIndex = newCurrentSceneIndex;
	}

	auto& getDefinition(std::string definitionId) {

		return m_sceneDefinitions[definitionId];
	}

	auto& scenes() {
		return m_scenes;
	}

	auto& playerInputEvents() {
		return m_PlayerInputEvents;
	}

	

private:
	
	//std::stack<std::unique_ptr<Scene>> m_scenes;
	std::vector<Scene> m_scenes;
	std::map<SDL_Keycode, SceneAction> m_globalKeyActions;
	std::vector<PlayerInputEvent> m_PlayerInputEvents;
	std::map<std::string, Json::Value>m_sceneDefinitions;

	int m_currentSceneIndex;
	

};

