#pragma once

#include <memory>
#include <vector>
#include <bitset>
#include <optional>

#include <box2d/box2d.h>
#include <json/json.h>

#include "Globals.h"
#include "GameObject.h"
#include "ParticleMachine.h"


struct SceneAction
{
	int actionCode;
	std::string sceneId;
};

class Scene
{

public:
	Scene(std::string sceneId);
	~Scene();

	void init(size_t mouseMode, std::string levelId, SDL_Keycode exitKey, size_t maxObjects);
	void run();
	void render();
	void update();
	std::string pollEvents();
	void clearEvents();
	void addGameObject(std::string gameObjectId, int layer, float xMapPos, float yMapPos, float angle=0., bool cameraFollow=false);
	void addGameObject(GameObject* gameObject, int layer);
	void addGameObject(std::shared_ptr<GameObject> gameObject, int layer);
	void addKeyAction(SDL_Keycode, SceneAction);
	void applyCurrentControlMode();


	std::string id() {
		return m_id;
	}
	ParticleMachine& particleMachine() {
		return m_particleMachine;
	}
	int parentSceneIndex() {
		return m_parentSceneIndex;
	}

	void setState(SceneState state) {
		m_state = state;
	}

	SceneState state() {
		return m_state;
	}
	void setInputControlMode(int inputControlMode);

	std::optional<SceneAction> getkeycodeAction(SDL_Keycode keycode) {
		if (m_sceneKeyActions.find(keycode) != m_sceneKeyActions.end()) {
			return m_sceneKeyActions.at(keycode);
		}
		else {
			return std::nullopt;
		}
	}
	
private:
	std::string m_id;
	SceneState m_state;
	int m_inputControlMode;
	std::bitset<8> m_sceneTags;
	ParticleMachine m_particleMachine {this};
	std::map<SDL_Keycode, SceneAction> m_sceneKeyActions;

	int m_parentSceneIndex;
	std::array <std::vector<std::shared_ptr<GameObject>>, MAX_GAMEOBJECT_LAYERS> m_gameObjects;




};

