#pragma once

#include <memory>
#include <vector>
#include <bitset>
#include <optional>

#include <box2d/box2d.h>
#include <json/json.h>
#include "Util.h"
#include "BaseConstants.h"
#include "GameObject.h"
#include "GameConfig.h"
#include "ObjectPoolManager.h"
#include "triggers/Trigger.h"

class CutScene;
struct Objective;

struct SceneAction
{
	int actionCode{ 0 };
	std::string actionId;
};

struct PhysicsConfig
{
	b2Vec2 gravity{};
	float timeStep{};
	int velocityIterations{};
	int positionIterations{};
	bool b2DebugDrawMode{false};
};

struct Parallax
{
	int layer{};
	float rate{1};
};

void _updatePhysics(b2World* physicsWorld);

class Scene
{

public:
	Scene(std::string sceneId);
	~Scene();

	void loadLevel(std::string levelId);
	void loadNextLevel();
	void reloadCurrentLevel();
	void reset();
	void clear();
	void render();
	void update();
	void clearEvents();

	GameObject* addGameObject(std::string gameObjectId, int layer, float xMapPos, float yMapPos, float angle = 0., bool cameraFollow = false,std::string name="");
	GameObject* addGameObject(std::string gameObjectId, int layer, PositionAlignment windowPosition, float adjustX=0, float adjustY=0, float angle=0, bool cameraFollow=false);
	GameObject* addGameObject(std::shared_ptr<GameObject> gameObject, int layer);
	void addLevelObjective(Objective objective);
	void addLevelTrigger(std::shared_ptr<Trigger> trigger);
	void addKeyAction(SDL_Keycode, SceneAction);
	void applyCurrentControlMode();
	SDL_FPoint calcWindowPosition(int globalPosition);
	GameObject* getGameObject(std::string name);
	
	void stepB2PhysicsWorld() {
		m_physicsWorld->Step(m_physicsConfig.timeStep,
			m_physicsConfig.velocityIterations,
			m_physicsConfig.positionIterations);
	}

	const std::array <std::vector<std::shared_ptr<GameObject>>, MAX_GAMEOBJECT_LAYERS>& gameObjects() {
		return m_gameObjects;
	}

	//Accessor Functions
	std::string id() {	return m_id;}
	int parentSceneIndex() { return m_parentSceneIndex; }
	b2World* physicsWorld() { return m_physicsWorld;	}
	int inputControlMode() { return m_inputControlMode; }
	const std::vector<Objective>& objectives() { return m_levelObjectives; }

	void setState(SceneState state) { m_state = state; }
	SceneState state() { return m_state; }

	std::optional<std::shared_ptr<CutScene>> cutScene() { return m_cutScene; }

	ObjectPoolManager& objectPoolManager() { return m_objectPoolManager; }
	PhysicsConfig physicsConfig() { return m_physicsConfig; }
	bool hasPhysics() { return m_hasPhysics; }
	int gameObjectCount() { return m_gameObjectCount; }
	void incrementGameObjectCount() { m_gameObjectCount += 1; }

	void setInputControlMode(int inputControlMode);
	std::optional<SceneAction> getkeycodeAction(SDL_Keycode keycode);
	void direct();
	void setCutScene(std::shared_ptr<CutScene>cutScene);
	void deleteCutScene();
	void addParallaxItem(Parallax& parallaxItem);
	
private:

	std::string m_id;

	int m_gameObjectCount{};
	int m_inputControlMode{};
	int m_parentSceneIndex{};
	bool m_hasPhysics{};

	SceneState m_state{};
	std::optional<std::shared_ptr<CutScene>> m_cutScene{};
	b2World* m_physicsWorld{};
	PhysicsConfig m_physicsConfig{};
	ObjectPoolManager m_objectPoolManager{};

	std::array <std::vector<std::shared_ptr<GameObject>>, MAX_GAMEOBJECT_LAYERS> m_gameObjects;
	std::bitset<8> m_sceneTags;
	std::map<SDL_Keycode, SceneAction> m_sceneKeyActions;

	//Level related members
	std::string m_currentLevelId;
	std::vector<std::shared_ptr<Trigger>> m_levelTriggers;
	std::vector<Objective> m_levelObjectives{};
	std::map<int, Parallax>m_parallaxRates{};

	void _processGameObjectInterdependecies();
	void _buildPhysicsWorld(Json::Value physicsJSON);
	void _buildSceneGameObjects(Json::Value sceneJSON);
	void _removeFromWorldPass();
	

};

