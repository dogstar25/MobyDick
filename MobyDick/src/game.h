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

#include "GameObjectContactListener.h"
#include "DebugDraw.h"
#include "DebugPanel.h"
#include "GameObjectCollection.h"

//forward declations
class PlayerObject;
class GameObject;
class ParticleObject;
class CompositeObject;
class WeaponObject;
class TextObject;

/*
	Main Game Class
*/
class Game {

private:
	GameObjectContactListener m_gameObjectContactListner;

public:

	Game();
	~Game();

	static Game& instance();
	bool init();
	void play();
	void handleEvents();

	static void renderCollection(std::array<GameObjectCollection, constants::MAX_GAMEOBJECT_LAYERS>&);
	
	void addGameObject(GameObject* gameObject, int);
	void addGameObject(TextObject* gameObject, int);
	void addGameObject(WorldObject* gameObject, int);
	void addGameObject(ParticleObject* gameObject, int);
	void addGameObject(CompositeObject* gameObject, int);
	void addGameObject(WeaponObject* gameObject, int);
	

	PlayerObject* player;

	std::unique_ptr<DebugPanel> debugPanel;

	//Current Game State
	int gameState;

	int fps;
	DebugDraw debugDraw;


	//Accessor Functions
	b2World* physicsWorld() {
		return m_physicsWorld;
	}
	SDL_Window* window() {
		return m_window;
	}

	std::array <GameObjectCollection, constants::MAX_GAMEOBJECT_LAYERS>& gameCollections() {

		return m_gameCollections;
	}


private:

	void _render();
	void _update();

	b2World* m_physicsWorld;
	SDL_Window* m_window;


	//Fixed array of Layers
	//Each layer contains a GameObjectCollection
	std::array <GameObjectCollection, constants::MAX_GAMEOBJECT_LAYERS> m_gameCollections;





};


#endif


