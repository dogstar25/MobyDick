#ifndef GAME_H
#define GAME_H

#include <Box2D/Box2D.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include <array>
#include <string>
#include <memory>

#include "Globals.h"
#include "GameObject.h"
//#include "GameObjectContactListener.h"
//#include "DebugDraw.h"
//#include "DebugPanel.h"
#include "GameObjectCollection.h"
#include "GameConfig.h"
#include "SceneManager.h"



/*
	Main Game Class
*/
class Game {

private:
	//GameObjectContactListener m_gameObjectContactListner;

public:

	Game();
	~Game();

	static Game& instance();
	bool init();
	void play();
	static void renderGameObjects(const std::array <std::vector<GameObject>, MAX_GAMEOBJECT_LAYERS>&);
	
	void addGameObject(std::shared_ptr<GameObject>gameObject, int layer);

	void setGameState(GameState state) {
		m_gameState = state;
	}
	void setWorldParams(SDL_Rect bounds, int worldTileWidth, int worldTileHeight)
	{
		m_WorldBounds = bounds;
		m_WorldTileWidth = worldTileWidth;
		m_WorldTileHeight = worldTileHeight;

	}
	void stepB2PhysicsWorld() {
		m_physicsWorld->Step(GameConfig::instance().timeStep(),
			GameConfig::instance().velocityIterations(),
			GameConfig::instance().positionIterations());
	}

	void setInputControlMode(int inputControlMode);

	//Accessor Functions
	b2World* physicsWorld() {
		return m_physicsWorld;
	}
	SDL_Window* window() {
		return m_window;
	}
	GameState gameState(){
		return m_gameState;
	}
	SDL_Rect worldBounds() {
		return m_WorldBounds;
	}
	int worldTileWidth() {
		return m_WorldTileWidth;
	}
	int worldTileHeight() {
		return m_WorldTileHeight;
	}

private:

	void _render();
	void _update();
	void _handleEvents();

	b2World* m_physicsWorld;
	SDL_Window* m_window;
	GameState m_gameState;
	SDL_Rect m_WorldBounds;
	int m_WorldTileWidth;
	int m_WorldTileHeight;

	/*Fixed array of Layers
	Each layer contains a vector of GameObjects
	For deletions, things with heavy number of deletes like bullets and particles should all be at 
	the end of the vector which shoudl make for acceptable erase performance
	*/
	std::array <std::vector<std::shared_ptr<GameObject>>, MAX_GAMEOBJECT_LAYERS> m_gameObjects;


};


#endif


