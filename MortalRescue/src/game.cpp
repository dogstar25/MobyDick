#include "Game.h"

#include "actions/ActionMaps.h"
#include "GameConstants.h"
#include "GameObjectManager.h"
#include "SoundManager.h"
#include "LevelManager.h"
#include "Camera.h"
#include "Clock.h"
#include "ContactFilter.h"
#include "ContactListener.h"
#include "components/DroneBrainComponent.h"
#include "particleEffects/ParticleEffectsMap.h"
#include "particleEffects/GameParticleEffects.h"

#include "EnumMaps.h"

#include "actions/MyAction.h"



using namespace std::chrono_literals;

Game& Game::instance()
{
	static Game singletonInstance;
	return singletonInstance;
}

Game::~Game()
{

	printf("cleaning game\n");

	//Delete SDL stuff
	SDL_DestroyWindow(m_window);
	SDL_Quit();
	TTF_Quit();

}

Game::Game()
{

	m_window = nullptr;
	

}

/*
Initialize Game
*/
bool Game::init()
{

	m_gameState = GameState::PLAY;

	//Add Game Specific stuff
	_addGameIdTags();
	_addGameActions();
	_addGameParticleEffects();

	//Get all of the configuration values
	GameConfig::instance().init("gameConfig");
	
	//Initialize world
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{

		//Init font library
		TTF_Init();

		//Create the game window
		uint16 windowFlags = 0 | SDL_WINDOW_OPENGL;
		if (GameConfig::instance().windowFullscreen() == true) 
		{
			windowFlags = windowFlags | SDL_WINDOW_FULLSCREEN;
		}
		else
		{
			windowFlags = windowFlags | SDL_WINDOW_RESIZABLE;
		}
		m_window = SDL_CreateWindow(GameConfig::instance().gameTitle().c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			GameConfig::instance().windowWidth(),
			GameConfig::instance().windowHeight(),
			windowFlags);

		//Initialize the texture manager
		Renderer::instance().init(m_window);

		//Initialize the texture manager
		TextureManager::instance().init();
		TextureManager::instance().load("textureAssets");

		//LevelManger
		//LevelManager::instance().init();

		//Initialize the SceneManager
		SceneManager::instance().init();
		SceneManager::instance().load("gameScenes");

		//Initialize the Game Object Manager
		GameObjectManager::instance().init();
		GameObjectManager::instance().load("gameObjectDefinitions/commonObjects");
		GameObjectManager::instance().load("levels/level1_Objects");
		GameObjectManager::instance().load("gameObjectDefinitions/particleObjects");
		GameObjectManager::instance().load("gameObjectDefinitions/compositeObjects");

		//Load a first scene
		Scene& scene = SceneManager::instance().pushScene("SCENE_PLAY");
		scene.applyCurrentControlMode();

		//Initialize the sound manager
		SoundManager::instance().initSound();
		SoundManager::instance().playMusic("music_ambience_1", -1);

		//Initialize the clock object
		Clock::instance().init();

		scene.addGameObject("FPS_VALUE", LAYER_MENU, 1, 1);

	}

	//test
	auto particleXEmitterObject = SceneManager::instance().addGameObject("PARTICLE_EMITTER_SPARK", LAYER_MAIN, 9, 9);

	return true;
}


/*
Main Play Loop
*/
void Game::play()
{

	while (m_gameState != GameState::QUIT)
	{

		std::optional<SceneAction> action = SceneManager::instance().pollEvents();

		if (action.has_value()) {

			if (action->actionCode == SCENE_ACTION_QUIT) {
				m_gameState = GameState::QUIT;
			}
			else if (action->actionCode == SCENE_ACTION_EXIT) {
				SceneManager::instance().popScene();
			}
			else if (action->actionCode == SCENE_ACTION_ADD) {
				SceneManager::instance().pushScene(action->sceneId);
			}
			else if (action->actionCode == SCENE_ACTION_REPLACE) {
				SceneManager::instance().popScene();
				SceneManager::instance().pushScene(action->sceneId);
			}

			//Apply the mouse control mode based on what the new "current" scene wants
			SceneManager::instance().scenes().back().applyCurrentControlMode();
		}

		SceneManager::instance().run();
	}

}

void Game::_addGameActions()
{

	ActionMaps::instance().addAction("MyAction", std::make_shared<MyAction>());

}

void Game::_addGameParticleEffects()
{

	ParticleEffectsMap::instance().addParticleEffect("ricochet", ParticleEffects::ricochet);
	ParticleEffectsMap::instance().addParticleEffect("ricochetX", ParticleEffects::ricochetX);
	ParticleEffectsMap::instance().addParticleEffect("deflect", ParticleEffects::deflect);
	ParticleEffectsMap::instance().addParticleEffect("scrap", ParticleEffects::scrap);
	ParticleEffectsMap::instance().addParticleEffect("spark", ParticleEffects::spark);

}

void Game::_addGameIdTags()
{

	//Add all game specific enum traslations needed
	EnumMap::instance().addEnumItem("IdTag::DEFLECT_EFFECT", IdTag::DEFLECT_EFFECT);
	EnumMap::instance().addEnumItem("IdTag::SMOKE_PARTICLE", IdTag::SMOKE_PARTICLE);
	EnumMap::instance().addEnumItem("IdTag::NAVIGATION_POINT", IdTag::NAVIGATION_POINT);
	EnumMap::instance().addEnumItem("IdTag::DRONE_BRAIN", IdTag::DRONE_BRAIN);
	EnumMap::instance().addEnumItem("IdTag::DRONE_SHIELD", IdTag::DRONE_SHIELD);
	EnumMap::instance().addEnumItem("IdTag::DRONE_WEAPON", IdTag::DRONE_WEAPON);
	EnumMap::instance().addEnumItem("IdTag::DRONE_FRAME", IdTag::DRONE_FRAME);
	EnumMap::instance().addEnumItem("IdTag::ENEMY_BULLET", IdTag::ENEMY_BULLET);
	EnumMap::instance().addEnumItem("IdTag::FRIENDLY_BULLET", IdTag::FRIENDLY_BULLET);
	EnumMap::instance().addEnumItem("IdTag::HEAVY_PARTICLE", IdTag::HEAVY_PARTICLE);
	EnumMap::instance().addEnumItem("IdTag::LIGHT_PARTICLE", IdTag::LIGHT_PARTICLE);
	EnumMap::instance().addEnumItem("IdTag::PLAYER", IdTag::PLAYER);;
	EnumMap::instance().addEnumItem("IdTag::PLAYER_BULLET", IdTag::PLAYER_BULLET);
	EnumMap::instance().addEnumItem("IdTag::SHIELD_SCRAP", IdTag::SHIELD_SCRAP);
	EnumMap::instance().addEnumItem("IdTag::SURVIVOR", IdTag::SURVIVOR);
	EnumMap::instance().addEnumItem("IdTag::MEDKIT", IdTag::MEDKIT);
	EnumMap::instance().addEnumItem("IdTag::WALL", IdTag::WALL);
	EnumMap::instance().addEnumItem("IdTag::WEAPON_PICKUP", IdTag::WEAPON_PICKUP);


}