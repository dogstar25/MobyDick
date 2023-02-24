//#pragma once
#ifndef BASE_CONSTANTS_H
#define BASE_CONSTANTS_H

#include <SDL2/SDL.h>

enum NavigationStatus {

	IN_PROGRESS = 0,
	DESTINATION_REACHED = 1,
	NO_PATH_FOUND = 2,
	STUCK = 3

};

inline constexpr int TURN_ON = 0;
inline constexpr int TURN_OFF = 1;

inline constexpr int CHILD_POSITIONS = 9;
inline constexpr int MAX_VITALITY_LEVELS = 5;

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

namespace NavigationSizeCategory {
	inline constexpr int SMALL = 0;
	inline constexpr int MEDIUM = 1;
	inline constexpr int LARGE = 2;

}

namespace DISABLED_TYPE {
	inline constexpr int RENDER = 0;
	inline constexpr int UPDATE = 1;
	inline constexpr int PHYSICS = 2;
	inline constexpr int RENDER_AND_PHYSICS = 3;
	inline constexpr int RENDER_AND_UPDATE = 4;
	inline constexpr int PHYICS_AND_UPDATE = 5;
}

namespace Colors {

	inline SDL_Color WHITE = { 255, 255, 255, 255 };
	inline SDL_Color BLACK = { 0, 0, 0, 255 };
	inline SDL_Color CLOUD = { 200, 200, 200, 200 };

	inline SDL_Color RED = { 255, 0, 0, 255 };
	inline SDL_Color GREEN = { 0, 255, 0, 255 };
	inline SDL_Color BLUE = { 0, 0, 255, 255 };
	inline SDL_Color YELLOW = { 255, 255, 0, 255 };
	inline SDL_Color PURPLE = { 255, 0, 255, 255 };
	inline SDL_Color ORANGE = { 255, 127, 0, 255 };
	inline SDL_Color GREY = { 127, 127, 127, 255 };
	inline SDL_Color BROWN = { 102, 57, 49, 255 };
	inline SDL_Color CYAN = { 0, 255, 255, 255 };
	


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
namespace ContactTag {
	inline constexpr int MAX_OBJECT_CATEGORIES = 56;

	//General
	inline constexpr int GENERAL_SOLID = 0;
	inline constexpr int GENERAL_FREE = 1;
	inline constexpr int LEVEL_CAGE = 2;
}

namespace TraitTag {

	inline constexpr int player = 1;
	inline constexpr int barrier = 2;
	inline constexpr int collectable = 3;
	inline constexpr int weapon = 4;
	inline constexpr int gui = 5;
	inline constexpr int waypoint = 6;
	inline constexpr int abstract = 7;
	inline constexpr int interactive = 8;
	inline constexpr int debug = 9;
	inline constexpr int pooled = 10;
	inline constexpr int fragment = 11;
	inline constexpr int objective = 12;
	inline constexpr int impasse = 13;
	inline constexpr int conditional_impasse = 14;
	inline constexpr int complex_impasse = 15;
	inline constexpr int mobile = 16;

}

namespace StateTag {

	inline constexpr int disabledUpdate = 1;
	inline constexpr int disabledPhysics = 2;
	inline constexpr int disabledRender = 3;
	inline constexpr int disabledCollision = 4;
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
	GLOW,
	count
};

enum class GLDrawerType {
	GLSPRITE = 0,
	GLLINE,
	GLPOINT,
	GLRECTANGLE,
	count
};

enum class PhysicsChainType {
	CCW_REFLECT_IN = 0,
	CW_REFLECT_OUT
};

namespace DebugSceneSettings {
	inline constexpr int SHOW_PHYSICS_DEBUG = 0;
	inline constexpr int SHOW_NAVIGATION_DEBUG_MAP = 1;
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
	inline constexpr int CONTAINER_COMPONENT = 7;
	inline constexpr int CHECKPOINT_COMPONENT = 8;
	inline constexpr int HUD_COMPONENT = 9;
	inline constexpr int INVENTORY_COMPONENT = 10;
	inline constexpr int IMGUI_COMPONENT = 11;
	inline constexpr int PARTICLE_COMPONENT = 12;
	inline constexpr int NAVIGATION_COMPONENT = 13;
	inline constexpr int PARTICLE_X_COMPONENT = 14;
	inline constexpr int PHYSICS_COMPONENT = 15;
	inline constexpr int PLAYER_CONTROL_COMPONENT = 16;
	inline constexpr int POOL_COMPONENT = 17;
	inline constexpr int RENDER_COMPONENT = 18;
	inline constexpr int SOUND_COMPONENT = 19;
	inline constexpr int TEXT_COMPONENT = 20;
	inline constexpr int TRANSFORM_COMPONENT = 21;
	inline constexpr int UICONTROL_COMPONENT = 22;
	inline constexpr int VITALITY_COMPONENT = 23;
	inline constexpr int WEAPON_COMPONENT = 24;
	
	inline constexpr int LAST_BASE_COMPONENT = 26;

}

//Animation States
inline constexpr int MAX_ANIMATION_STATES = 20;
namespace AnimationState {

	inline constexpr int IDLE = 0;
	inline constexpr int RUN = 1;
	inline constexpr int ACTIVE = 2;
	inline constexpr int ACTION = 3;
	inline constexpr int DEPLOY = 4;
	inline constexpr int UNDEPLOY = 5;
	inline constexpr int DEAD = 6;
	inline constexpr int SPRINT = 7;
}

//Animation Modes
inline constexpr int ANIMATE_ONE_TIME = 0;
inline constexpr int ANIMATE_CONTINUOUS = 1;

//Player Control
inline constexpr int INPUT_CONTROL_MOVEMENT = 1;
inline constexpr int INPUT_CONTROL_USE = 2;
inline constexpr int INPUT_CONTROL_HOVER = 3;
inline constexpr int INPUT_CONTROL_CLICK = 4;

//Actions
namespace Actions {
	inline constexpr int NONE = 0;
	inline constexpr int MOVE = 1;
	inline constexpr int ROTATE = 2;
	inline constexpr int USE = 3;
	inline constexpr int USAGE = 4;
	inline constexpr int INTERACT = 5;
	inline constexpr int INTERACTION = 6;
	inline constexpr int ON_HOVER = 7;
	inline constexpr int ON_CLICK = 8;
	inline constexpr int ON_HOVER_OUT = 9;
	inline constexpr int ON_TRIGGER = 10;
	inline constexpr int SPRINT = 11;
	inline constexpr int USAGE_SPECIAL = 12;
	
}

//GameSpace Types
inline constexpr int GAMESPACE_INTRO = 0;
inline constexpr int GAMESPACE_PLAY = 1;
inline constexpr int GAMESPACE_GAMEOVER = 2;
inline constexpr int GAMESPACE_MENU = 3;

//Scene Action Codes
inline constexpr int SCENE_ACTION_QUIT = 0;
inline constexpr int SCENE_ACTION_ADD = 1;
inline constexpr int SCENE_ACTION_ADD_AND_PAUSE = 2;
inline constexpr int SCENE_ACTION_REPLACE = 3;
inline constexpr int SCENE_ACTION_EXIT = 4;
inline constexpr int SCENE_ACTION_LOAD_LEVEL = 5;
inline constexpr int SCENE_ACTION_LOAD_NEXTLEVEL = 6;
inline constexpr int SCENE_ACTION_LOAD_CURRENTLEVEL = 7;
inline constexpr int SCENE_ACTION_DIRECT = 8;
inline constexpr int SCENE_ACTION_RELEASE_DIRECT = 9;
inline constexpr int SCENE_ACTION_RESPAWN_PLAYER = 10;
inline constexpr int SCENE_ACTION_TOGGLE_SETTING = 11;
inline constexpr int SCENE_ACTION_WINDOW_PAUSE = 12;
inline constexpr int SCENE_ACTION_WINDOW_UNPAUSE = 13;

//Scene Tags
inline constexpr int SCENETAG_MENU = 1;

//Game Layers
inline constexpr int MAX_GAMEOBJECT_LAYERS = 8;

namespace GameLayer {
	inline constexpr int BACKGROUND_1 = 0;
	inline constexpr int BACKGROUND_2 = 1;
	inline constexpr int MAIN = 2;
	inline constexpr int FOREGROUND_1 = 3;
	inline constexpr int FOREGROUND_2 = 4;
	inline constexpr int GUI = 5;
	inline constexpr int ABSTRACT = 6;
	inline constexpr int GRID_DISPLAY = 7;
}

//Game Object Display Modes
inline constexpr int MAX_GAMEOBJECT_DISPLAY_UI_MODES = 3;
inline constexpr int DISPLAY_UI_MODE_STANDARD = 0;
inline constexpr int DISPLAY_UI_MODE_ONHOVER = 1;
inline constexpr int DISPLAY_UI_MODE_ONCLICK = 2;

//Game Object Display Schemes
inline constexpr int DEFAULT_HOVER_SCHEME = 0;

namespace StatusItemId {

	inline constexpr int DEFAULT_EMPTY = 0;
	inline constexpr int CURRENT_LEVEL = 1;


};



#endif