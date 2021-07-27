#include "StubGame.h"

#include "actions/ActionMaps.h"
#include "GameConstants.h"
#include "GameObjectManager.h"
#include "SoundManager.h"
#include "LevelManager.h"
#include "Camera.h"
#include "Clock.h"
#include "ContactFilter.h"
#include "ContactListener.h"
#include "particleEffects/ParticleEffectsMap.h"
#include "particleEffects/GameParticleEffects.h"

#include "EnumMaps.h"

using namespace std::chrono_literals;


StubGame::StubGame()
{

}

StubGame::~StubGame()
{
	
}

/*
Initialize Game
*/
bool StubGame::init(ContactListener* contactListener, ContactFilter* contactFilter,
	ComponentFactory* componentFactory)
{
	//Required Base Class calls
	Game::init(contactListener, contactFilter, componentFactory);
	//End Required

	std::cout << "Stubb Begins\n";

	m_gameState = GameState::PLAY;

	//Add Game Specific stuff
	_addGameCollisionTags();
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

		//Display basic loading message
		_displayLoadingMsg();

		//Initialize the texture manager
		TextureManager::instance().init();
		TextureManager::instance().load("textureAssets");

		//Initialize the SceneManager
		SceneManager::instance().init();
		SceneManager::instance().load("gameScenes");

		_displayLoadingMsg();

		//Initialize the Game Object Manager
		GameObjectManager::instance().init();
		GameObjectManager::instance().load("gameObjectDefinitions/commonObjects");

		//Load a first scene
		Scene& scene = SceneManager::instance().pushScene("SCENE_PLAY");

		_displayLoadingMsg();

		//Initialize the sound manager
		SoundManager::instance().initSound();
		SoundManager::instance().playMusic("music_ambience_1", -1);

		//Initialize the clock object
		Clock::instance().init();

		scene.addGameObject("FPS_VALUE", LAYER_MENU, 1, 1);
	}

	return true;
}


/*
Main Play Loop
*/
void StubGame::play()
{
	while (m_gameState != GameState::QUIT) {

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
		}

		SceneManager::instance().run();
	}
}

void StubGame::_addGameActions()
{


	

}

void StubGame::_addGameParticleEffects()
{
	ParticleEffectsMap::instance().addParticleEffect("whaleBubble", ParticleEffects::whaleBubble);
	ParticleEffectsMap::instance().addParticleEffect("shipSmoke", ParticleEffects::shipSmoke);

}

void StubGame::_addGameCollisionTags()
{

	//Add all game specific enum traslations needed
	EnumMap::instance().addEnumItem("CollisionTag::STUB", CollisionTag::STUBB);

}

