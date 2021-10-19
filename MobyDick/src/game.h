#ifndef GAME_H
#define GAME_H

#include <box2d/box2d.h>
#include <jsoncpp/include/json/json.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include <array>
#include <string>
#include <memory>

#include "Globals.h"
#include "BaseConstants.h"
#include "GameObject.h"
#include "DebugDraw.h"
#include "GameConfig.h"
#include "SceneManager.h"
#include "ComponentFactory.h"
#include "ContactListener.h"
#include "ContactFilter.h"
#include "Renderer.h"


/*
	Main Game Class
*/
class Game {

public:

	Game() = default;
	~Game();

	virtual bool init();
	virtual bool init(ContactListener*, ContactFilter*,	ComponentFactory* ) = 0;
	virtual void play();
	virtual void _displayLoadingMsg();
	
	void setGameState(GameState state) {
		m_gameState = state;
	}
	void setWorldParams(SDL_Rect bounds, int worldTileWidth, int worldTileHeight)
	{
		m_WorldBounds = bounds;
		m_WorldTileWidth = worldTileWidth;
		m_WorldTileHeight = worldTileHeight;
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
	ContactListener* contactListener() {
		return m_contactListener;
	}
	ContactFilter* contactFilter() {
		return m_contactFilter;
	}
	ComponentFactory* componentFactory() {
		return m_componentFactory;
	}

	Renderer* renderer() { return m_renderer.get(); }

protected:

	SDL_Window* m_window{};
	GameState m_gameState{};
	SDL_Rect m_WorldBounds{};
	ContactListener* m_contactListener{};
	ContactFilter* m_contactFilter{};
	ComponentFactory* m_componentFactory{};

	std::shared_ptr<Renderer> m_renderer;

	int m_WorldTileWidth{};
	int m_WorldTileHeight{};

	virtual void _addGameCollisionTags() = 0;
	virtual void _addGameActions() = 0;
	virtual void _addGameParticleEffects() = 0;
	

};


#endif


