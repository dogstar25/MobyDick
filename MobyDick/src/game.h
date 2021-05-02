#ifndef GAME_H
#define GAME_H

#include <box2d/box2d.h>
#include <json/json.h>

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



/*
	Main Game Class
*/
class Game {

private:

public:

	Game();
	~Game();

	static Game& instance();
	virtual bool init();
	virtual void play();
	
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

private:

	SDL_Window* m_window;
	GameState m_gameState;
	SDL_Rect m_WorldBounds;
	int m_WorldTileWidth;
	int m_WorldTileHeight;

	void _addGameIdTags();
	void _addGameActions();
	void _addGameParticleEffects();
};


#endif


