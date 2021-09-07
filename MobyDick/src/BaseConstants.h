//#pragma once
#ifndef BASE_CONSTANTS_H
#define BASE_CONSTANTS_H



inline constexpr int CHILD_POSITIONS = 9;
inline constexpr int MAX_VITALITY_LEVELS = 5;
inline constexpr int MAX_ANIMATION_STATES = 5;
inline constexpr int DESTINATION_DISTANCE_TOLERANCE = 10;
inline constexpr int PARTICLE_EMITTER_FORCE_ADJ = 50;
inline constexpr int MAX_OBJECT_TYPES = 5;
inline constexpr int MAX_GL_TEXTURES = 1;
inline constexpr int MAX_GL_SHADER_TYPES = 5;

//Mouse State
inline constexpr int MOUSE_NONE = 0;
inline constexpr int MOUSE_HOVER = 1;
inline constexpr int MOUSE_HOLD = 2;
inline constexpr int MOUSE_CLICKED = 3;

//Mouse Modes
inline constexpr int CONTROL_MODE_SELECT = 0;
inline constexpr int CONTROL_MODE_PLAY = 1;

//Game Object Collision Tags
namespace CollisionTag {
	inline constexpr int MAX_OBJECT_CATEGORIES = 32;

	//General
	inline constexpr int GENERAL_SOLID = 0;
	inline constexpr int GENERAL_FREE = 1;
}

namespace TraitTag {

	static const int player = 1;
	static const int barrier = 2;
	static const int collectable = 3;
	static const int weapon = 4;
	static const int gui = 5;
	static const int   navigation;

}

namespace GameObjectType {

	inline constexpr auto ABSTRACT = 0; // SOMETHING LIKE A NAVIGATION POINT THAT DOES NOT DISPLAY
	inline constexpr auto SPRITE = 1;
	inline constexpr auto LINE = 2;
	inline constexpr auto POINT = 3;
	inline constexpr auto RECTANGLE = 4;

}

enum RendererType {


	OPENGL = 0,
	SDL,

	count
};

enum class GLDrawerType {

	GLSPRITE = 0,
	GLLINE,
	GLPOINT,
	GLRECTANGLE,

	count

};

namespace BrainState {

	inline constexpr auto IDLE = 0;
	inline constexpr auto PATROL = 1;
	inline constexpr auto ALERT = 2;
	inline constexpr auto PURSUE = 3;
	inline constexpr auto ENGAGE = 4;
	inline constexpr auto FLEE = 5;

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

//Components
enum class ComponentTypes {

	ACTION_COMPONENT = 0,
	ANIMATION_COMPONENT,
	ATTACHMENTS_COMPONENT,
	BRAIN_COMPONENT,
	CHILDREN_COMPONENT,
	COMPOSITE_COMPONENT,
	INVENTORY_COMPONENT,
	PARTICLE_COMPONENT,
	NAVIGATION_COMPONENT,
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
inline constexpr int ACTION_ON_TRIGGER = 9;

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