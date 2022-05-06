//#pragma once
#ifndef BASE_CONSTANTS_H
#define BASE_CONSTANTS_H

#include <SDL2/SDL.h>


inline constexpr int CHILD_POSITIONS = 9;
inline constexpr int MAX_VITALITY_LEVELS = 5;
inline constexpr int MAX_ANIMATION_STATES = 12;
inline constexpr int DESTINATION_DISTANCE_TOLERANCE = 25;
inline constexpr int PARTICLE_EMITTER_FORCE_ADJ = 50;
inline constexpr int PRACTICLE_MOVE_SPEED_ADJ = 50;
inline constexpr int MAX_OBJECT_TYPES = 5;
//OpenGl
inline constexpr int MAX_GL_TEXTURES = 1;
//inline constexpr int MAX_GL_SHADER_TYPES = 5;

//Mouse State
inline constexpr int MOUSE_NONE = 0;
inline constexpr int MOUSE_HOVER = 1;
inline constexpr int MOUSE_HOLD = 2;
inline constexpr int MOUSE_CLICKED = 3;

//Mouse Modes
inline constexpr int CONTROL_MODE_SELECT = 0;
inline constexpr int CONTROL_MODE_PLAY = 1;
inline constexpr int CONTROL_MODE_IMGUI = 2;


namespace Colors {

	inline SDL_Color WHITE = { 255, 255, 255, 255 };
	inline SDL_Color BLACK = { 0, 0, 0, 255 };
	inline SDL_Color RED = { 255, 0, 0, 255 };
	inline SDL_Color GREEN = { 0, 255, 0, 255 };
	inline SDL_Color BLUE = { 0, 0, 255, 255 };
	inline SDL_Color YELLOW = { 255, 255, 0, 255 };
	inline SDL_Color PURPLE = { 255, 0, 255, 255 };

}

namespace Alpha {

	inline int ALPHA25 = { 64 };
	inline int ALPHA50 = { 128 };
	inline int ALPHA65 = { 167 };
	inline int ALPHA75 = { 192 };
	inline int ALPHA85 = { 218 };
	inline int ALPHA100 = { 256 };

}


//Game Object Collision Tags
namespace CollisionTag {
	inline constexpr int MAX_OBJECT_CATEGORIES = 32;

	//General
	inline constexpr int GENERAL_SOLID = 0;
	inline constexpr int GENERAL_FREE = 1;
}

namespace TraitTag {

	inline constexpr int player = 1;
	inline constexpr int barrier = 2;
	inline constexpr int collectable = 3;
	inline constexpr int weapon = 4;
	inline constexpr int gui = 5;
	inline constexpr int navigation = 6;
	inline constexpr int abstract = 7;
	inline constexpr int interactive = 8;

}

namespace GameObjectType {

	inline constexpr auto ABSTRACT = 0; // SOMETHING LIKE A NAVIGATION POINT THAT DOES NOT DISPLAY
	inline constexpr auto SPRITE = 1;
	inline constexpr auto LINE = 2;
	inline constexpr auto POINT = 3;
	inline constexpr auto RECTANGLE = 4;

}


namespace BrainState {

	inline constexpr auto IDLE = 0;
	inline constexpr auto DISPATCH = 1;
	inline constexpr auto PATROL = 2;
	inline constexpr auto ALERT = 3;
	inline constexpr auto PURSUE = 4;
	inline constexpr auto ENGAGE = 5;
	inline constexpr auto FLEE = 6;
	inline constexpr auto DEPLOY = 7;
	inline constexpr auto UNDEPLOY = 8;

}

namespace CameraState {

	inline constexpr auto IDLE = 0;
	inline constexpr auto DISPATCH = 1;
	inline constexpr auto FOLLOW = 2;
}

namespace ParticleEmitterType {

	inline constexpr int ONETIME = 0;
	inline constexpr int CONTINUOUS = 1;
}

namespace NavigationObjectType {

	inline constexpr int UNSPECIFIED= 0;
	inline constexpr int TRANSIT_POINT = 1;
	inline constexpr int WAYPOINT = 2;
}

//namespace PositionAlignment {
//
//	inline constexpr int CENTER = 0;
//	inline constexpr int TOP_LEFT = 1;
//	inline constexpr int TOP_CENTER = 2;
//	inline constexpr int TOP_RIGHT = 3;
//	inline constexpr int CENTER_LEFT = 4;
//	inline constexpr int CENTER_RIGHT = 5;
//	inline constexpr int BOTTOM_LEFT = 6;
//	inline constexpr int BOTTOM_CENTER = 7;
//	inline constexpr int BOTTOM_RIGHT = 8;
//
//}

enum class PositionAlignment {

	CENTER = 0,
	TOP_LEFT = 1,
	TOP_CENTER = 2,
	TOP_RIGHT = 3,
	CENTER_LEFT = 4,
	CENTER_RIGHT = 5,
	BOTTOM_LEFT = 6,
	BOTTOM_CENTER = 7,
	BOTTOM_RIGHT = 8,

};

enum class GameState {
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

enum class RendererType{
	OPENGL = 0,
	SDL,
	count
};

enum class GLShaderType {
	BASIC = 0,
	UBER,
	LINE,
	count
};

enum class GLDrawerType {
	GLSPRITE = 0,
	GLLINE,
	GLPOINT,
	GLRECTANGLE,
	count
};


//Component Types
namespace ComponentTypes {
	inline constexpr int MAX_COMPONENT_TYPES = 32;

	inline constexpr int NONE = 0;
	inline constexpr int ACTION_COMPONENT = 1;
	inline constexpr int ANIMATION_COMPONENT = 2;
	inline constexpr int ATTACHMENTS_COMPONENT = 3;
	inline constexpr int BRAIN_COMPONENT = 4;
	inline constexpr int CHILDREN_COMPONENT = 5;
	inline constexpr int COMPOSITE_COMPONENT = 6;
	inline constexpr int HUD_COMPONENT = 7;
	inline constexpr int INVENTORY_COMPONENT = 8;
	inline constexpr int IMGUI_COMPONENT = 9;
	inline constexpr int PARTICLE_COMPONENT = 10;
	inline constexpr int NAVIGATION_COMPONENT = 11;
	inline constexpr int PARTICLE_X_COMPONENT = 12;
	inline constexpr int PHYSICS_COMPONENT = 13;
	inline constexpr int PLAYER_CONTROL_COMPONENT = 14;
	inline constexpr int POOL_COMPONENT = 15;
	inline constexpr int RENDER_COMPONENT = 16;
	inline constexpr int TEXT_COMPONENT = 17;
	inline constexpr int TRANSFORM_COMPONENT = 18;
	inline constexpr int UICONTROL_COMPONENT = 19;
	inline constexpr int VITALITY_COMPONENT = 20;
	inline constexpr int WEAPON_COMPONENT = 21;
	
	inline constexpr int LAST_BASE_COMPONENT = 22;

}



//Components
//enum class ComponentTypes {
//	ACTION_COMPONENT = 0,
//	ANIMATION_COMPONENT,
//	ATTACHMENTS_COMPONENT,
//	BRAIN_COMPONENT,
//	CHILDREN_COMPONENT,
//	COMPOSITE_COMPONENT,
//	HUD_COMPONENT,
//	INVENTORY_COMPONENT,
//	PARTICLE_COMPONENT,
//	NAVIGATION_COMPONENT,
//	PARTICLE_X_COMPONENT,
//	PHYSICS_COMPONENT,
//	PLAYERCONTROL_COMPONENT,
//	POOL_COMPONENT,
//	RENDER_COMPONENT,
//	TEXT_COMPONENT,
//	TRANSFORM_COMPONENT,
//	UICONTROL_COMPONENT,
//	VITALITY_COMPONENT,
//	WEAPON_COMPONENT,
//	COUNT
//};

//Animation States
inline constexpr int ANIMATION_IDLE = 0;
inline constexpr int ANIMATION_RUN = 1;
inline constexpr int ANIMATION_ACTIVE = 2;
inline constexpr int ANIMATION_ACTION = 3;
inline constexpr int ANIMATION_DEPLOY = 4;
inline constexpr int ANIMATION_UNDEPLOY = 5;

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
inline constexpr int ACTION_INTERACTION = 6;
inline constexpr int ACTION_ON_HOVER = 7;
inline constexpr int ACTION_ON_CLICK = 8;
inline constexpr int ACTION_ON_HOVER_OUT = 9;
inline constexpr int ACTION_ON_TRIGGER = 10;

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
inline constexpr int SCENE_ACTION_DIRECT = 4;
inline constexpr int SCENE_ACTION_RELEASE_DIRECT = 5;

//Scene Tags
inline constexpr int SCENETAG_MENU = 1;

//Game Layers
inline constexpr int MAX_GAMEOBJECT_LAYERS = 7;
inline constexpr int LAYER_BACKGROUND_1 = 0;
inline constexpr int LAYER_BACKGROUND_2 = 1;
inline constexpr int LAYER_MAIN = 2;
inline constexpr int LAYER_FOREGROUND_1 = 3;
inline constexpr int LAYER_FOREGROUND_2 = 4;
inline constexpr int LAYER_MENU = 5;
inline constexpr int LAYER_ABSTRACT = 6;

//Game Object Display Modes
inline constexpr int MAX_GAMEOBJECT_DISPLAY_UI_MODES = 3;
inline constexpr int DISPLAY_UI_MODE_STANDARD = 0;
inline constexpr int DISPLAY_UI_MODE_ONHOVER = 1;
inline constexpr int DISPLAY_UI_MODE_ONCLICK = 2;

//Game Object Display Schemes
inline constexpr int DEFAULT_HOVER_SCHEME = 0;

#endif