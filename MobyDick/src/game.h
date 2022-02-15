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

#include "Util.h"
#include "BaseConstants.h"
#include "GameObject.h"
#include "DebugDraw.h"
#include "GameConfig.h"
#include "SceneManager.h"
#include "ComponentFactory.h"
#include "cutScenes/CutSceneFactory.h"
#include "actions/ActionFactory.h"
#include "IMGui/IMGuiFactory.h"
#include "particleEffects/ParticleEffectsFactory.h"
#include "ContactListener.h"
#include "ContactFilter.h"
#include "StatusManager.h"
#include "Renderer.h"


/*
	Main Game Class
*/
class Game {

public:

	Game() = default;
	~Game();

	virtual bool init();
	virtual bool init(ContactListener*, ContactFilter*,	ComponentFactory*, ActionFactory*, ParticleEffectsFactory*, 
		CutSceneFactory*, IMGuiFactory*, StatusManager*) = 0;
	virtual void play();
	virtual void _displayLoadingMsg();
	GameObject* getGameObject(std::string name);
	
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
	std::shared_ptr <ContactListener> contactListener() {
		return m_contactListener;
	}
	std::shared_ptr<ContactFilter> contactFilter() {
		return m_contactFilter;
	}
	std::shared_ptr<ComponentFactory> componentFactory() {
		return m_componentFactory;
	}
	std::shared_ptr<ActionFactory> actionFactory() {
		return m_actionFactory;
	}
	std::shared_ptr<CutSceneFactory> cutSceneFactory() {
		return m_cutSceneFactory;
	}
	std::shared_ptr<StatusManager> statusMananger() {
		return m_statusMananger;
	}
	std::shared_ptr<ParticleEffectsFactory> particleEffectsFactory() {
		return m_particleEffectsFactory;
	}
	std::shared_ptr<IMGuiFactory> iMGUIFactory() {
		return m_iMGUIFactory;
	}

	Renderer* renderer() { return m_renderer.get(); }

	bool IMGuiControlled=false;

protected:

	SDL_Window* m_window{};
	GameState m_gameState{};
	SDL_Rect m_WorldBounds{};
	std::shared_ptr<ContactListener> m_contactListener{};
	std::shared_ptr<ContactFilter> m_contactFilter{};
	std::shared_ptr<ComponentFactory> m_componentFactory{};
	std::shared_ptr<ActionFactory> m_actionFactory{};
	std::shared_ptr<CutSceneFactory> m_cutSceneFactory{};
	std::shared_ptr<StatusManager> m_statusMananger{};
	std::shared_ptr<ParticleEffectsFactory> m_particleEffectsFactory{};
	std::shared_ptr<IMGuiFactory> m_iMGUIFactory{};

	std::shared_ptr<Renderer> m_renderer;

	int m_WorldTileWidth{};
	int m_WorldTileHeight{};

	virtual void _addGameCollisionTags() {};
	virtual void _addGameComponentTypes() {};

	
	

};


#endif


