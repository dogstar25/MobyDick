#include "EnumMaps.h"
#include "hud/HudItemFactory.h"
#include "Renderer.h"

#include <box2d/box2d.h>

EnumMap& EnumMap::instance()
{

	static EnumMap singletonInstance;
	return singletonInstance;

}
EnumMap::EnumMap()
{

	//Renderer Types
	m_enumMap["RenderType::OPENGL"] = (int)RendererType::OPENGL;
	m_enumMap["RenderType::SDL"] = (int)RendererType::SDL;

	//Render Modes
	m_enumMap["RenderBlendMode::BLEND"] = (int)RenderBlendMode::BLEND;
	m_enumMap["RenderBlendMode::ADD"] = (int)RenderBlendMode::ADD;
	m_enumMap["RenderBlendMode::NONE"] = (int)RenderBlendMode::NONE;

	//Mouse State
	m_enumMap["MOUSE_NONE"] = MOUSE_NONE;
	m_enumMap["MOUSE_HOVER"] = MOUSE_HOVER;
	m_enumMap["MOUSE_NONE"] = MOUSE_HOLD;
	m_enumMap["MOUSE_HOVER"] = MOUSE_CLICKED;

	//Mouse Modes
	m_enumMap["CONTROL_MODE_SELECT"] = CONTROL_MODE_SELECT;
	m_enumMap["CONTROL_MODE_PLAY"] = CONTROL_MODE_PLAY;

	//Renderer Types
	m_enumMap["RendererType::SDL"] = (int)RendererType::SDL;
	m_enumMap["RendererType::OPENGL"] = (int)RendererType::OPENGL;

	//GameObject CollisionTags
	m_enumMap["CollisionTag::GENERAL_SOLID"] = CollisionTag::GENERAL_SOLID;
	m_enumMap["CollisionTag::GENERAL_FREE"] = CollisionTag::GENERAL_FREE;

	//Box2d Related
	m_enumMap["B2_STATIC"] = b2_staticBody;
	m_enumMap["B2_KINEMATIC"] = b2_kinematicBody;
	m_enumMap["B2_DYNAMIC"] = b2_dynamicBody;
	m_enumMap["B2_CIRCLE"] = b2Shape::e_circle;
	m_enumMap["B2_BOX"] = b2Shape::e_polygon;
	m_enumMap["B2_CHAIN"] = b2Shape::e_chain;
	m_enumMap["B2_WELD"] = b2JointType::e_weldJoint;
	m_enumMap["B2_REVOLUTE"] = b2JointType::e_revoluteJoint;


	//Animation Statessdpssss
	m_enumMap["ANIMATION_IDLE"] = ANIMATION_IDLE;
	m_enumMap["ANIMATION_RUN"] = ANIMATION_RUN;
	m_enumMap["ANIMATION_ACTIVE"] = ANIMATION_ACTIVE;
	m_enumMap["ANIMATION_ACTION"] = ANIMATION_ACTION;

	//Player Control
	m_enumMap["INPUT_CONTROL_MOVEMENT"] = INPUT_CONTROL_MOVEMENT;
	m_enumMap["INPUT_CONTROL_USE"] = INPUT_CONTROL_USE;
	m_enumMap["INPUT_CONTROL_HOVER"] = INPUT_CONTROL_HOVER;
	m_enumMap["INPUT_CONTROL_CLICK"] = INPUT_CONTROL_CLICK;

	//Game Object Actions
	m_enumMap["ACTION_NONE"] = ACTION_NONE;
	m_enumMap["ACTION_MOVE"] = ACTION_MOVE;
	m_enumMap["ACTION_ROTATE"] = ACTION_ROTATE;
	m_enumMap["ACTION_USE"] = ACTION_USE;
	m_enumMap["ACTION_USAGE"] = ACTION_USAGE;
	m_enumMap["ACTION_INTERACT"] = ACTION_INTERACT;
	m_enumMap["ACTION_ON_HOVER"] = ACTION_ON_HOVER;
	m_enumMap["ACTION_ON_HOVER_OUT"] = ACTION_ON_HOVER_OUT;
	m_enumMap["ACTION_ON_CLICK"] = ACTION_ON_CLICK;

	//Keys
	m_enumMap["SDLK_ESCAPE"] = SDLK_ESCAPE;

	//Scene Action Codes
	m_enumMap["SCENE_ACTION_QUIT"] = SCENE_ACTION_QUIT;
	m_enumMap["SCENE_ACTION_ADD"] = SCENE_ACTION_ADD;
	m_enumMap["SCENE_ACTION_REPLACE"] = SCENE_ACTION_REPLACE;
	m_enumMap["SCENE_ACTION_EXIT"] = SCENE_ACTION_EXIT;

	//Scene Tags
	m_enumMap["SCENETAG_MENU"] = SCENETAG_MENU;

	//Object Alignments
	m_enumMap["PositionAlignment::TOP_LEFT"] = (int)PositionAlignment::TOP_LEFT;
	m_enumMap["PositionAlignment::TOP_CENTER"] = (int)PositionAlignment::TOP_CENTER;
	m_enumMap["PositionAlignment::TOP_RIGHT"] = (int)PositionAlignment::TOP_RIGHT;
	m_enumMap["PositionAlignment::CENTER_LEFT"] = (int)PositionAlignment::CENTER_LEFT;
	m_enumMap["PositionAlignment::CENTER"] = (int)PositionAlignment::CENTER;
	m_enumMap["PositionAlignment::CENTER_RIGHT"] = (int)PositionAlignment::CENTER_RIGHT;
	m_enumMap["PositionAlignment::BOTTOM_LEFT"] = (int)PositionAlignment::BOTTOM_LEFT;
	m_enumMap["PositionAlignment::BOTTOM_CENTER"] = (int)PositionAlignment::BOTTOM_CENTER;
	m_enumMap["PositionAlignment::BOTTOM_RIGHT"] = (int)PositionAlignment::BOTTOM_RIGHT;

	//Game Layers
	m_enumMap["LAYER_BACKGROUND_1"] = LAYER_BACKGROUND_1;
	m_enumMap["LAYER_BACKGROUND_2"] = LAYER_BACKGROUND_2;
	m_enumMap["LAYER_MAIN"] = LAYER_MAIN;
	m_enumMap["LAYER_FOREGROUND_1"] = LAYER_FOREGROUND_1;
	m_enumMap["LAYER_FOREGROUND_2"] = LAYER_FOREGROUND_2;
	m_enumMap["LAYER_MENU"] = LAYER_MENU;
	m_enumMap["LAYER_ABSTRACT"] = LAYER_ABSTRACT;

	//Game Object Display Modes
	m_enumMap["DISPLAY_UI_MODE_STANDARD"] = DISPLAY_UI_MODE_STANDARD;
	m_enumMap["DISPLAY_UI_MODE_ONHOVER"] = DISPLAY_UI_MODE_ONHOVER;
	m_enumMap["DISPLAY_UI_MODE_ONCLICK"] = DISPLAY_UI_MODE_ONCLICK;

	//Particle Emitter types
	m_enumMap["ParticleEmitterType::ONETIME"] = ParticleEmitterType::ONETIME;
	m_enumMap["ParticleEmitterType::CONTINUOUS"] = ParticleEmitterType::CONTINUOUS;

	//Navigation Object types
	m_enumMap["NavigationObjectType::UNSPECIFIED"] = NavigationObjectType::UNSPECIFIED;
	m_enumMap["NavigationObjectType::TRANSIT_POINT"] = NavigationObjectType::TRANSIT_POINT;
	m_enumMap["NavigationObjectType::WAYPOINT"] = NavigationObjectType::WAYPOINT;

	//Texure Blend Modes
	//m_enumMap["SDL_BLENDMODE_BLEND"] = SDL_BLENDMODE_BLEND;
	//m_enumMap["SDL_BLENDMODE_ADD"] = SDL_BLENDMODE_ADD;
	//m_enumMap["SDL_BLENDMODE_NONE"] = SDL_BLENDMODE_NONE;

	//GameObject Types
	//m_enumMap["GameObjectType::ABSTRACT"] = GameObjectType::ABSTRACT;
	//m_enumMap["GameObjectType::SPRITE"] = GameObjectType::SPRITE;
	//m_enumMap["GameObjectType::LINE"] = GameObjectType::LINE;
	//m_enumMap["GameObjectType::POINT"] = GameObjectType::POINT;
	//m_enumMap["GameObjectType::RECTANGLE"] = GameObjectType::RECTANGLE;

	//GameObject Trait Tags
	m_enumMap["player"] = TraitTag::player;
	m_enumMap["barrier"] = TraitTag::barrier;
	m_enumMap["collectable"] = TraitTag::collectable;
	m_enumMap["weapon"] = TraitTag::weapon;
	m_enumMap["gui"] = TraitTag::gui;
	m_enumMap["navigation"] = TraitTag::navigation;
	m_enumMap["abstract"] = TraitTag::abstract;

	//Hud Item Types
	m_enumMap["HudItemTypes::STATUS_SINGLE"] = (int)HudItemTypes::STATUS_SINGLE;
	m_enumMap["HudItemTypes::STATUS_SERIES"] = (int)HudItemTypes::STATUS_SERIES;

}

EnumMap::~EnumMap()
{
	m_enumMap.clear();
}

const int EnumMap::toEnum(std::string name)
{
	assert(m_enumMap.find(name) != m_enumMap.end() && "Constant Name wasnt found in EnumMap");

	return m_enumMap[name];
}


