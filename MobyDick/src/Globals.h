#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
#include <map>

#include <json/json.h>
#include <SDL2/SDL.h>

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

//Mouse State
inline constexpr int MOUSE_NONE = 0;
inline constexpr int MOUSE_HOVER = 1;
inline constexpr int MOUSE_HOLD = 2;
inline constexpr int MOUSE_CLICKED = 3;

//Mouse Modes
inline constexpr int CONTROL_MODE_SELECT = 0;
inline constexpr int CONTROL_MODE_PLAY = 1;

//Collision Groups
inline constexpr int COLLISION_GENERIC = 1;
inline constexpr int COLLISION_PLAYER = 2;
inline constexpr int COLLISION_WALL = 4;
inline constexpr int COLLISION_PLAYER_BULLET = 8;
inline constexpr int COLLISION_PARTICLE1 = 16;
inline constexpr int COLLISION_PARTICLE2 = 32;
inline constexpr int COLLISION_PARTICLE3 = 64;
inline constexpr int COLLISION_ENEMY_FRAME = 128;
inline constexpr int COLLISION_ENEMY_ARMOR = 256;
inline constexpr int COLLISION_ENEMY_ARMOR_PIECE = 512;

//Components
inline constexpr int ACTION_COMPONENT = 1;
inline constexpr int ANIMATION_COMPONENT = 2;
inline constexpr int ATTACHMENTS_COMPONENT = 3;
inline constexpr int CHILDREN_COMPONENT = 4;
inline constexpr int COMPOSITE_COMPONENT = 5;
inline constexpr int TRANSFORM_COMPONENT = 6;
inline constexpr int PARTICLE_COMPONENT = 7;
inline constexpr int PHYSICS_COMPONENT = 8;
inline constexpr int RENDER_COMPONENT = 9;
inline constexpr int TEXT_COMPONENT = 10;
inline constexpr int VITALITY_COMPONENT = 11;
inline constexpr int WEAPON_COMPONENT = 12;
inline constexpr int PLAYERCONTROL_COMPONENT = 13;
inline constexpr int INVENTORY_COMPONENT = 14;

//Animation States
inline constexpr int ANIMATION_IDLE = 0;
inline constexpr int ANIMATION_RUN = 1;
inline constexpr int ANIMATION_ACTIVE = 3;
inline constexpr int ANIMATION_ACTION = 4;

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
inline constexpr int ACTION_INTERACT = 4;
inline constexpr int ACTION_ON_HOVER = 5;
inline constexpr int ACTION_ON_CLICK = 6;
inline constexpr int ACTION_ON_HOVER_OUT = 7;

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
	const float radiansToDegrees(float angleInRadians);
	const float degreesToRadians(float angleInDegrees);
	const std::string floatToString(float x, int decDigits);
	const SDL_Color JsonToColor(Json::Value JsonColor);

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