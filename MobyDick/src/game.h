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
	static void renderGameObjects(const std::array <std::vector<GameObject>, constants::MAX_GAMEOBJECT_LAYERS>&);
	
	void addGameObject(std::string gameObjectId, int layer, float xMapPos, float yMapPos, float angle);
	void setGameState(int state) {
		m_gameState = state;
	}
	
	//Accessor Functions
	b2World* physicsWorld() {
		return m_physicsWorld;
	}
	SDL_Window* window() {
		return m_window;
	}
	GameObject* player() {
		return m_player;
	}
	int gameState(){
		return m_gameState;
	}

private:

	void _render();
	void _update();
	void _handleEvents();


	b2World* m_physicsWorld;
	SDL_Window* m_window;
	GameObject* m_player;
	int m_gameState;

	/*Fixed array of Layers
	Each layer contains a vector of GameObjects
	For deletions, things with heavy number of deletes like bullets and particles should all be at 
	the end of the vector which shoudl make for acceptable erase performance
	*/
	std::array <std::vector<std::shared_ptr<GameObject>>, constants::MAX_GAMEOBJECT_LAYERS> m_gameObjects;


};


#endif


