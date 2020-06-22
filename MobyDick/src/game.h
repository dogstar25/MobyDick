#ifndef GAME_H
#define GAME_H

#include <Box2D/Box2D.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include <array>
#include <string>
#include <memory>

#include "Constants.h"

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

	void play();
	bool init();
	void render();
	static void renderCollection(std::array<GameObjectCollection, constants::MAX_GAMEOBJECT_LAYERS>*);
	void update();
	void handleEvents();
	
	void addGameObject(GameObject* gameObject, int);
	void addGameObject(TextObject* gameObject, int);
	void addGameObject(WorldObject* gameObject, int);
	void addGameObject(ParticleObject* gameObject, int);
	void addGameObject(CompositeObject* gameObject, int);
	void addGameObject(WeaponObject* gameObject, int);
	static Game& instance();

	b2World* physicsWorld;
	
	//Main screen and window stuff
	SDL_Window* pWindow;
	int gameObjectCount;

	//mouse
	b2Vec2 mouseLocation, mouseClickLocation;

	//Fixed array of Layers
	//Each layer contains a GameObjectCollection
	std::array <GameObjectCollection, constants::MAX_GAMEOBJECT_LAYERS> gameCollections;
	PlayerObject* player;

	std::unique_ptr<DebugPanel> debugPanel;

	//Current Game State
	int gameState;

	int fps;
	DebugDraw debugDraw;


	//Accessor Functions



};


#endif


