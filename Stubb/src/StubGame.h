#ifndef STUBGAME_H
#define STUBGAME_H

#include <box2d/box2d.h>
#include <json/json.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include <array>
#include <string>
#include <memory>

#include "game.h"
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
class StubGame : public Game {

public:

	StubGame();
	~StubGame();

	bool init() override { return true; }
	bool init(ContactListener*, ContactFilter*, ComponentFactory* ) override;

private:

	void _addGameCollisionTags() override;
	void _addGameActions() override;
	void _addGameParticleEffects() override;

};


#endif


