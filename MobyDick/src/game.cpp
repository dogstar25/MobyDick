#include "Game.h"

#include "GameObjectManager.h"
#include "SoundManager.h"
#include "Camera.h"
#include "Clock.h"
#include "ContactFilter.h"
#include "ContactListener.h"
#include "config_data/GameDefinitions.h"



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
	

	for (int x = 0; x < MAX_GAMEOBJECT_LAYERS; x++)
	{
		m_gameObjects[x].clear();
	}

	////Delete box2d world - should delete all bodies and fixtures within
	//delete m_physicsWorld;

	TTF_Quit();


}

Game::Game()
{

	//m_physicsWorld = nullptr;

	m_window = nullptr;

}

/*
Initialize Game
*/
bool Game::init()
{

	//string it = todd::testit[0].id;
	//SceneConfigData::scenes;

	//std::string test = todd::testit[0].id;

	m_gameState = GameState::PLAY;

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
		TextureManager::instance().init(m_window);

		//Initialize the SceneManager
		SceneManager::instance().init();

		//Initialize the Game Object Manager
		GameObjectManager::instance().init();

		//Load a first scene
		Scene& scene = SceneManager::instance().pushScene("SCENE_PLAY");
		scene.applyCurrentControlMode();

		//Initialize the sound manager
		//SoundManager::instance().initSound();
		//SoundManager::instance().playMusic("MUSIC_AMBIENCE_1", -1);

		// Construct a physics world object, which will hold and simulate the physics objects.
		//m_physicsWorld = new b2World(GameConfig::instance().gravity());
		//m_physicsWorld->SetAutoClearForces(true);

		////Add a collision contact listener and filter for box2d callbacks
		//m_physicsWorld->SetContactListener(&ContactListener::instance());
		//m_physicsWorld->SetContactFilter(&ContactFilter::instance());

		/*
		m_physicsWorld->RayCast(&RayCastListener, x, x, x);

		RayCastListener will derive from b2RayCastCallback
		b2RayCastCallback has a virtual function called ReportFixture that we will override
		and have it examine each fixture that it returns, comparing them until we find the one that 
		is closest. We can ignore ones that can be seen through

		Add this Raycast call to the BrainComponents update function

		
		*/

		//Initialize the clock object
		Clock::instance().init();

		scene.addGameObject("FPS_VALUE", LAYER_MENU, 1, 1);
		scene.addGameObject("DRONE", LAYER_MAIN, 2, 2);
	}

	//Load the player and some other objects
	//scene.addGameObject("GINA_64", LAYER_MAIN, 8, 8, 0, true);

	auto particleXEmitterObject = Game::instance().addGameObject("PARTICLE_EMITTER_SPARK", LAYER_MAIN, 9, 9);

	//auto particleXEmitterObject = Game::instance().addGameObject("PARTICLE_X_EMITTER", LAYER_MAIN, 9, 9);
	//auto& particleXComponent = particleXEmitterObject->getComponent<ParticleXComponent>(ComponentTypes::PARTICLE_X_COMPONENT);
	//particleXComponent->addParticleEffect(ParticleEffects::spark);
	//particleXComponent->setType(ParticleEmitterType::CONTINUOUS);
	//particleXComponent->setEmissionInterval(std::chrono::duration<float>(.1));


	//auto particleEmitterObject = Game::instance().addGameObject("PARTICLE_EMITTER", LAYER_MAIN, 15, 15);
	//auto& particleComponent = particleEmitterObject->getComponent<ParticleComponent>(ComponentTypes::PARTICLE_COMPONENT);
	//particleComponent->addParticleEffect(ParticleEffects::ricochet);
	//particleComponent->setType(ParticleEmitterType::CONTINUOUS);
	//particleComponent->setEmissionInterval(std::chrono::duration<float>(.5));



	#define gina_64 = GameDefs::instance().gina_64;

	


	/*for (int x = 0; x < 10000; x++) {

		int xpos = util::generateRandomNumber(1, 32);
		int ypos = util::generateRandomNumber(1, 32);
		int angle = util::generateRandomNumber(1, 360);
		scene.addGameObject("BOWMAN", LAYER_MAIN, xpos, ypos, angle, false);
	}*/



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

void Game::setInputControlMode(int inputControlMode)
{
	if (inputControlMode == CONTROL_MODE_PLAY) {

		SDL_ShowCursor(false);
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
	else if(inputControlMode == CONTROL_MODE_SELECT) {
		SDL_ShowCursor(true);
		SDL_SetRelativeMouseMode(SDL_FALSE);

	}
}

GameObject* Game::addGameObject(std::shared_ptr<GameObject>gameObject, int layer)
{
	//Add the gameObject to the currently active scene using back()
	auto gameObjectRef = SceneManager::instance().scenes().back().addGameObject(gameObject, layer);

	return gameObjectRef;


}

GameObject* Game::addGameObject(std::string gameObjectId, int layer, float xMapPos, float yMapPos, float angle, bool cameraFollow)
{
	//Add the gameObject to the currently active scene using back()
	auto& currentScene = SceneManager::instance().scenes().back();
	auto gameObject = currentScene.addGameObject(gameObjectId, layer, xMapPos, yMapPos, angle, cameraFollow);

	return gameObject;

}





