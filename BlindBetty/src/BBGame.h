#ifndef BBGAME_H
#define BBGAME_H

#include <box2d/box2d.h>
#include <json/json.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include <array>
#include <string>
#include <memory>

#include "game.h"
#include "Util.h"
#include "BaseConstants.h"
#include "GameObject.h"
#include "DebugDraw.h"
#include "GameConfig.h"
#include "SceneManager.h"
#include "ComponentFactory.h"
#include "actions/ActionFactory.h"
#include "particleEffects/ParticleEffectsFactory.h"
#include "IMGui/IMGuiFactory.h"
#include "triggers/TriggerFactory.h"
#include "ContextManager.h"

/*
	Main Game Class
*/
class BBGame : public Game {

public:

	BBGame() = default;
	//~MRGame();

	static BBGame& instance();

	bool init(std::shared_ptr<ContactListener>, std::shared_ptr<ContactFilter>,
		std::shared_ptr<ComponentFactory>, std::shared_ptr<ActionFactory>, std::shared_ptr<ParticleEffectsFactory>,
		std::shared_ptr<CutSceneFactory>, std::shared_ptr<IMGuiFactory>, std::shared_ptr<TriggerFactory>, std::shared_ptr<ContextManager>,
		std::shared_ptr<EnumMap> enumMap, std::shared_ptr<ColorMap> colorMap) override;

private:

};


#endif


