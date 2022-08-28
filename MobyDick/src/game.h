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
#include "triggers/TriggerFactory.h"
#include "particleEffects/ParticleEffectsFactory.h"
#include "ContactListener.h"
#include "ContactFilter.h"
#include "ContextManager.h"
#include "Renderer.h"
#include "EnumMap.h"
#include "ColorMap.h"


/*
	Main Game Class
*/
class Game {

public:

	Game() = default;
	~Game();

	virtual bool init(std::shared_ptr<ContactListener>, std::shared_ptr<ContactFilter>,
		std::shared_ptr<ComponentFactory>, std::shared_ptr<ActionFactory>, std::shared_ptr<ParticleEffectsFactory>,
		std::shared_ptr<CutSceneFactory>, std::shared_ptr<IMGuiFactory>, std::shared_ptr<TriggerFactory>, std::shared_ptr<ContextManager>,
		std::shared_ptr<EnumMap>, std::shared_ptr<ColorMap>) = 0;

	virtual void play();
	virtual void _displayLoadingMsg();
	//std::optional<std::shared_ptr<GameObject>> getGameObjectByName(GameObject* gameObject);
	
	void setGameState(GameState state) {
		m_gameState = state;
	}
	void setWorldParams(SDL_Rect gameScreenResolution, SDL_Point gameTileSize);

	SDL_Point gameScreenResolution() { return m_gameScreenResolution; }

	SDL_Window* window() {
		return m_window;
	}
	GameState gameState() {
		return m_gameState;
	}
	SDL_Point worldTileSize() {
		return m_worldTileSize;
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
	std::shared_ptr<ContextManager> contextMananger() {
		return m_contextMananger;
	}
	std::shared_ptr<ParticleEffectsFactory> particleEffectsFactory() {
		return m_particleEffectsFactory;
	}
	std::shared_ptr<IMGuiFactory> iMGUIFactory() {
		return m_iMGUIFactory;
	}
	std::shared_ptr<TriggerFactory> triggerFactory() {
		return m_triggerFactory;
	}
	std::shared_ptr<EnumMap> enumMap() {
		return m_enumMap;
	}
	std::shared_ptr<ColorMap> colorMap() {
		return m_colorMap;
	}

	Renderer* renderer() { return m_renderer.get(); }

	bool IMGuiControlled = false;

protected:

	SDL_Window* m_window{};
	GameState m_gameState{};
	SDL_Rect m_worldBounds{};
	SDL_Point m_gameScreenResolution{};
	SDL_Point m_worldTileSize{};
	std::shared_ptr<ContactListener> m_contactListener{};
	std::shared_ptr<ContactFilter> m_contactFilter{};
	std::shared_ptr<ComponentFactory> m_componentFactory{};
	std::shared_ptr<ActionFactory> m_actionFactory{};
	std::shared_ptr<CutSceneFactory> m_cutSceneFactory{};
	std::shared_ptr<ContextManager> m_contextMananger{};
	std::shared_ptr<ParticleEffectsFactory> m_particleEffectsFactory{};
	std::shared_ptr<IMGuiFactory> m_iMGUIFactory{};
	std::shared_ptr<TriggerFactory> m_triggerFactory{};
	std::shared_ptr<EnumMap> m_enumMap{};
	std::shared_ptr<ColorMap> m_colorMap{};

	std::shared_ptr<Renderer> m_renderer;

	std::optional<SDL_Point> _determineScreenResolution();
	

};


#endif


