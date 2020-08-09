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


constexpr int MAX_GAMEOBJECT_LAYERS = 3;
constexpr int CHILD_POSITIONS = 9;

//Mouse State
constexpr int MOUSE_NONE = 0;
constexpr int MOUSE_HOVER = 1;
constexpr int MOUSE_HOLD = 2;
constexpr int MOUSE_CLICKED = 3;

//Mouse Modes
constexpr int CONTROL_MODE_SELECT = 0;
constexpr int CONTROL_MODE_PLAY = 1;

//Collision Groups
constexpr int COLLISION_GENERIC = 1;
constexpr int COLLISION_PLAYER = 2;
constexpr int COLLISION_WALL = 4;
constexpr int COLLISION_PLAYER_BULLET = 8;
constexpr int COLLISION_PARTICLE1 = 16;
constexpr int COLLISION_PARTICLE2 = 32;
constexpr int COLLISION_PARTICLE3 = 64;
constexpr int COLLISION_ENEMY_FRAME = 128;
constexpr int COLLISION_ENEMY_ARMOR = 256;
constexpr int COLLISION_ENEMY_ARMOR_PIECE = 512;

//Components
constexpr int ACTION_COMPONENT = 1;
constexpr int ANIMATION_COMPONENT = 2;
constexpr int ATTACHMENTS_COMPONENT = 3;
constexpr int CHILDREN_COMPONENT = 4;
constexpr int COMPOSITE_COMPONENT = 5;
constexpr int TRANSFORM_COMPONENT = 6;
constexpr int PARTICLE_COMPONENT = 7;
constexpr int PHYSICS_COMPONENT = 8;
constexpr int RENDER_COMPONENT = 9;
constexpr int TEXT_COMPONENT = 10;
constexpr int VITALITY_COMPONENT = 11;
constexpr int WEAPON_COMPONENT = 12;
constexpr int PLAYERCONTROL_COMPONENT = 13;
constexpr int INVENTORY_COMPONENT = 14;

//Animation States
constexpr int ANIMATION_IDLE = 0;
constexpr int ANIMATION_RUN = 1;
constexpr int ANIMATION_ACTIVE = 3;
constexpr int ANIMATION_ACTION = 4;

//Animation Modes
constexpr int ANIMATE_ONE_TIME = 0;
constexpr int ANIMATE_CONTINUOUS = 1;

//Player Control
constexpr int INPUT_CONTROL_MOVEMENT = 1;
constexpr int INPUT_CONTROL_USE = 2;
constexpr int INPUT_CONTROL_HOVER = 3;
constexpr int INPUT_CONTROL_CLICK = 4;

//Actions
constexpr int ACTION_NONE = 0;
constexpr int ACTION_MOVE = 1;
constexpr int ACTION_ROTATE = 2;
constexpr int ACTION_USE = 3;
constexpr int ACTION_INTERACT = 4;
constexpr int ACTION_ON_HOVER = 5;
constexpr int ACTION_ON_CLICK = 6;

//GameSpace Types
constexpr int GAMESPACE_INTRO = 0;
constexpr int GAMESPACE_PLAY = 1;
constexpr int GAMESPACE_GAMEOVER = 2;
constexpr int GAMESPACE_MENU = 3;

//Scene Action Codes
constexpr int SCENE_ACTION_QUIT = 0;
constexpr int SCENE_ACTION_ADD = 1;
constexpr int SCENE_ACTION_REPLACE = 2;
constexpr int SCENE_ACTION_EXIT = 3;

//Scene Tags
constexpr int SCENETAG_MENU = 1;

//Game Layers
constexpr int LAYER_BACKGROUND = 0;
constexpr int LAYER_MAIN = 1;
constexpr int LAYER_TEXT = 2;


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



#endif