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
#include "ecs/Coordinator.hpp"
#include "systems/RenderSystem.h"
#include "systems/PhysicsSystem.h"





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
	
	void addGameObject(std::string gameObjectId, int layer, float xMapPos, float yMapPos, float angle);
	void addGameObject(std::shared_ptr<GameObject>gameObject, int layer);

	void setGameState(int state) {
		m_gameState = state;
	}
	void setWorldParams(SDL_Rect bounds, int worldTileWidth, int worldTileHeight)
	{
		m_WorldBounds = bounds;
		m_WorldTileWidth = worldTileWidth;
		m_WorldTileHeight = worldTileHeight;

	}

	//Accessor Functions
	Coordinator& gameCoordinator() {
		return m_gameCoordinator;
	}
	b2World* physicsWorld() {
		return m_physicsWorld;
	}
	SDL_Window* window() {
		return m_window;
	}
	int gameState(){
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
	int m_gameState;

	SDL_Rect m_WorldBounds;
	int m_WorldTileWidth;
	int m_WorldTileHeight;

	//Main game ECS coordinator
	Coordinator m_gameCoordinator;

	//Entity systems
	std::shared_ptr<RenderSystem> m_renderSystem;
	std::shared_ptr<PhysicsSystem> m_physicsSystem;



};


#endif


