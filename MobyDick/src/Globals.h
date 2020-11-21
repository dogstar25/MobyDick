#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
#include <map>

#include <json/json.h>
#include <SDL2/SDL.h>
#include <box2d/box2d.h>

/*
Overloaded operators used throught the game
*/
static bool operator ==(SDL_Color a, SDL_Color b)
{
	return (a.r == b.r) && (a.g == b.g) && (a.b == b.b);
}

static bool operator !=(SDL_Color a, SDL_Color b)
{
	return (a.r != b.r) || (a.g != b.g) || (a.b != b.b);
}


inline constexpr int MAX_GAMEOBJECT_LAYERS = 3;
inline constexpr int CHILD_POSITIONS = 9;
inline constexpr int MAX_VITALITY_LEVELS = 5;
inline constexpr int MAX_ANIMATION_STATES = 5;

//Mouse State
inline constexpr int MOUSE_NONE = 0;
inline constexpr int MOUSE_HOVER = 1;
inline constexpr int MOUSE_HOLD = 2;
inline constexpr int MOUSE_CLICKED = 3;

//Mouse Modes
inline constexpr int CONTROL_MODE_SELECT = 0;
inline constexpr int CONTROL_MODE_PLAY = 1;

//Game Object Id Tags
namespace IdTag {
	inline constexpr int MAX_OBJECT_CATEGORIES = 32;

	//General
	inline constexpr int GENERAL = 0;
	//Intangible
	inline constexpr int DEFLECT_EFFECT = 1;
	inline constexpr int SMOKE_PARTICLE = 2;
	//Non-stationary
	inline constexpr int DRONE_BRAIN = 3;
	inline constexpr int DRONE_FRAME = 4;
	inline constexpr int DRONE_SHIELD = 5;
	inline constexpr int DRONE_WEAPON = 6;
	inline constexpr int ENEMY_BULLET = 7;
	inline constexpr int FRIENDLY_BULLET = 8;
	inline constexpr int GUI = 9;
	inline constexpr int HEAVY_PARTICLE = 10;
	inline constexpr int LIGHT_PARTICLE = 11;
	inline constexpr int PLAYER = 12;
	inline constexpr int PLAYER_BULLET = 13;
	inline constexpr int SHIELD_SCRAP = 14;
	inline constexpr int SURVIVOR = 15;
	//Stationary
	inline constexpr int MEDKIT = 16;
	inline constexpr int WALL = 17;
	inline constexpr int WEAPON_PICKUP = 18;

}

namespace IdTagLabels {

	//General
	inline constexpr auto GENERAL = "General";
	//Intangible
	inline constexpr auto DEFLECT_EFFECT = "DeflectEffect";
	inline constexpr auto SMOKE_PARTICLE = "SmokeParticle";;
	//Non-stationary
	inline constexpr auto DRONE_BRAIN = "DroneBrain";
	inline constexpr auto DRONE_FRAME = "DroneFrame";
	inline constexpr auto DRONE_SHIELD = "DroneShield";
	inline constexpr auto DRONE_WEAPON = "DroneWeapon";
	inline constexpr auto ENEMY_BULLET = "EnemyBullet";
	inline constexpr auto FRIENDLY_BULLET = "FriendlyBullet";
	inline constexpr auto HEAVY_PARTICLE = "HeavyParticle";
	inline constexpr auto LIGHT_PARTICLE = "LightParticle";
	inline constexpr auto PLAYER = "Player";
	inline constexpr auto PLAYER_BULLET = "PlayerBullet";
	inline constexpr auto SHIELD_SCRAP = "ShieldScrap";
	inline constexpr auto SURVIVOR = "Survivor";
	//Stationary
	inline constexpr auto MEDKIT = "Medkit";
	inline constexpr auto WALL = "Wall";
	inline constexpr auto WEAPON_PICKUP = "WeaponPickup";

}

namespace ParticleEmitterType {

	inline constexpr int ONETIME = 0;
	inline constexpr int CONTINUOUS = 1;
}

//Level Location Item Types
namespace LevelLocItemType {

	inline constexpr int GAMEOBJECT = 0;
	inline constexpr int WAYPOINT = 1;

}

//Components
enum class ComponentTypes {

	ACTION_COMPONENT = 0,
	ANIMATION_COMPONENT,
	ATTACHMENTS_COMPONENT,
	CHILDREN_COMPONENT,
	COMPOSITE_COMPONENT,
	INVENTORY_COMPONENT,
	PARTICLE_COMPONENT,
	PARTICLE_X_COMPONENT,
	PHYSICS_COMPONENT,
	PLAYERCONTROL_COMPONENT,
	POOL_COMPONENT,
	RENDER_COMPONENT,
	TEXT_COMPONENT,
	TRANSFORM_COMPONENT,
	UICONTROL_COMPONENT,
	VITALITY_COMPONENT,
	WEAPON_COMPONENT,

	COUNT
};

//Animation States
inline constexpr int ANIMATION_IDLE = 0;
inline constexpr int ANIMATION_RUN = 1;
inline constexpr int ANIMATION_ACTIVE = 2;
inline constexpr int ANIMATION_ACTION = 3;

//Animation Modes
inline constexpr int ANIMATE_ONE_TIME = 0;
inline constexpr int ANIMATE_CONTINUOUS = 1;

//Player Control
inline constexpr int INPUT_CONTROL_MOVEMENT = 1;
inline constexpr int INPUT_CONTROL_USE = 2;
inline constexpr int INPUT_CONTROL_HOVER = 3;
inline constexpr int INPUT_CONTROL_CLICK = 4;

//Actions
inline constexpr int ACTION_NONE = 0;
inline constexpr int ACTION_MOVE = 1;
inline constexpr int ACTION_ROTATE = 2;
inline constexpr int ACTION_USE = 3;
inline constexpr int ACTION_USAGE = 4;
inline constexpr int ACTION_INTERACT = 5;
inline constexpr int ACTION_ON_HOVER = 6;
inline constexpr int ACTION_ON_CLICK = 7;
inline constexpr int ACTION_ON_HOVER_OUT = 8;

//GameSpace Types
inline constexpr int GAMESPACE_INTRO = 0;
inline constexpr int GAMESPACE_PLAY = 1;
inline constexpr int GAMESPACE_GAMEOVER = 2;
inline constexpr int GAMESPACE_MENU = 3;

//Scene Action Codes
inline constexpr int SCENE_ACTION_QUIT = 0;
inline constexpr int SCENE_ACTION_ADD = 1;
inline constexpr int SCENE_ACTION_REPLACE = 2;
inline constexpr int SCENE_ACTION_EXIT = 3;

//Scene Tags
inline constexpr int SCENETAG_MENU = 1;

//Game Layers
inline constexpr int LAYER_BACKGROUND = 0;
inline constexpr int LAYER_MAIN = 1;
inline constexpr int LAYER_TEXT = 2;

//Game Object Display Modes
inline constexpr int MAX_GAMEOBJECT_DISPLAY_UI_MODES = 3;
inline constexpr int DISPLAY_UI_MODE_STANDARD = 0;
inline constexpr int DISPLAY_UI_MODE_ONHOVER = 1;
inline constexpr int DISPLAY_UI_MODE_ONCLICK = 2;

//Game Object Display Schemes
inline constexpr int DEFAULT_HOVER_SCHEME = 0;

namespace util
{
	const int generateRandomNumber(int min, int max);
	const float generateRandomNumber(float min, float max);
	const SDL_Color generateRandomColor();
	const SDL_Color generateRandomColor(SDL_Color beginRange, SDL_Color endRange);
	const float radiansToDegrees(float angleInRadians);
	const float degreesToRadians(float angleInDegrees);
	const std::string floatToString(float x, int decDigits);
	const SDL_Color JsonToColor(Json::Value JsonColor);

	b2Vec2 matchParentRotation(SDL_FPoint childPosition, SDL_FPoint parentPosition, float);

};

/*
Game State
*/
enum class GameState{ 
	QUIT = 0,
	PLAY = 1,
	PAUSE = 2,
	EXIT = 3
};

enum class SceneState {
	RUN = 0,
	PAUSE = 1,
	EXIT = 2
};

namespace WindowPosition {

	inline constexpr int CENTER = 0;
	inline constexpr int TOP_LEFT = 1;
	inline constexpr int TOP_CENTER = 2;
	inline constexpr int TOP_RIGHT = 3;
}



#endif