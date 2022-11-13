#include "EnumMap.h"
#include "hud/HudItemFactory.h"
#include "Renderer.h"
#include "opengl/GLRenderer.h"
#include "components/PhysicsComponent.h"
#include "ContextManager.h"


EnumMap::EnumMap()
{

	//Component Types
	m_enumMap["ACTION_COMPONENT"] = (int)ComponentTypes::ACTION_COMPONENT;
	m_enumMap["ANIMATION_COMPONENT"] = (int)ComponentTypes::ANIMATION_COMPONENT;
	m_enumMap["ATTACHMENTS_COMPONENT"] = (int)ComponentTypes::ATTACHMENTS_COMPONENT;
	m_enumMap["BRAIN_COMPONENT"] = (int)ComponentTypes::BRAIN_COMPONENT;
	m_enumMap["CHECKPOINT_COMPONENT"] = (int)ComponentTypes::CHECKPOINT_COMPONENT;
	m_enumMap["CHILDREN_COMPONENT"] = (int)ComponentTypes::CHILDREN_COMPONENT;
	m_enumMap["COMPOSITE_COMPONENT"] = (int)ComponentTypes::COMPOSITE_COMPONENT;
	m_enumMap["CONTAINER_COMPONENT"] = (int)ComponentTypes::CONTAINER_COMPONENT;
	m_enumMap["HUD_COMPONENT"] = (int)ComponentTypes::HUD_COMPONENT;
	m_enumMap["INVENTORY_COMPONENT"] = (int)ComponentTypes::INVENTORY_COMPONENT;
	m_enumMap["IMGUI_COMPONENT"] = (int)ComponentTypes::IMGUI_COMPONENT;
	m_enumMap["PARTICLE_COMPONENT"] = (int)ComponentTypes::PARTICLE_COMPONENT;
	m_enumMap["NAVIGATION_COMPONENT"] = (int)ComponentTypes::NAVIGATION_COMPONENT;
	m_enumMap["PARTICLE_X_COMPONENT"] = (int)ComponentTypes::PARTICLE_X_COMPONENT;
	m_enumMap["PHYSICS_COMPONENT"] = (int)ComponentTypes::PHYSICS_COMPONENT;
	m_enumMap["PLAYER_CONTROL_COMPONENT"] = (int)ComponentTypes::PLAYER_CONTROL_COMPONENT;
	m_enumMap["POOL_COMPONENT"] = (int)ComponentTypes::POOL_COMPONENT;
	m_enumMap["RENDER_COMPONENT"] = (int)ComponentTypes::RENDER_COMPONENT;
	m_enumMap["SOUND_COMPONENT"] = (int)ComponentTypes::SOUND_COMPONENT;
	m_enumMap["TEXT_COMPONENT"] = (int)ComponentTypes::TEXT_COMPONENT;
	m_enumMap["TRANSFORM_COMPONENT"] = (int)ComponentTypes::TRANSFORM_COMPONENT;
	m_enumMap["UICONTROL_COMPONENT"] = (int)ComponentTypes::UICONTROL_COMPONENT;
	m_enumMap["VITALITY_COMPONENT"] = (int)ComponentTypes::VITALITY_COMPONENT;
	m_enumMap["WEAPON_COMPONENT"] = (int)ComponentTypes::WEAPON_COMPONENT;


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
	m_enumMap["CONTROL_MODE_IMGUI"] = CONTROL_MODE_IMGUI;

	//Renderer Types
	m_enumMap["RendererType::SDL"] = (int)RendererType::SDL;
	m_enumMap["RendererType::OPENGL"] = (int)RendererType::OPENGL;

	//GameObject CollisionTags
	m_enumMap["ContactTag::GENERAL_SOLID"] = ContactTag::GENERAL_SOLID;
	m_enumMap["ContactTag::GENERAL_FREE"] = ContactTag::GENERAL_FREE;
	m_enumMap["ContactTag::LEVEL_CAGE"] = ContactTag::LEVEL_CAGE;

	//Box2d Related
	m_enumMap["B2_STATIC"] = b2_staticBody;
	m_enumMap["B2_KINEMATIC"] = b2_kinematicBody;
	m_enumMap["B2_DYNAMIC"] = b2_dynamicBody;
	m_enumMap["B2_CIRCLE"] = b2Shape::e_circle;
	m_enumMap["B2_BOX"] = b2Shape::e_polygon;
	m_enumMap["B2_CHAIN"] = b2Shape::e_chain;
	m_enumMap["B2_EDGE"] = b2Shape::e_edge;
	m_enumMap["B2_WELD"] = b2JointType::e_weldJoint;
	m_enumMap["B2_REVOLUTE"] = b2JointType::e_revoluteJoint;

	//Physics chain winding order to reflec outwards or inwards
	m_enumMap["PhysicsChainType::CCW_REFLECT_IN"] = (int)PhysicsChainType::CCW_REFLECT_IN;
	m_enumMap["PhysicsChainType::CW_REFLECT_OUT"] = (int)PhysicsChainType::CW_REFLECT_OUT;

	//Animation Statessdpssss
	m_enumMap["ANIMATION_IDLE"] = ANIMATION_IDLE;
	m_enumMap["ANIMATION_RUN"] = ANIMATION_RUN;
	m_enumMap["ANIMATION_ACTIVE"] = ANIMATION_ACTIVE;
	m_enumMap["ANIMATION_ACTION"] = ANIMATION_ACTION;
	m_enumMap["ANIMATION_DEPLOY"] = ANIMATION_DEPLOY;
	m_enumMap["ANIMATION_UNDEPLOY"] = ANIMATION_UNDEPLOY;
	m_enumMap["ANIMATION_DEAD"] = ANIMATION_DEAD;
	m_enumMap["ANIMATION_SPRINT"] = ANIMATION_SPRINT;

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
	m_enumMap["ACTION_INTERACTION"] = ACTION_INTERACTION;
	m_enumMap["ACTION_ON_HOVER"] = ACTION_ON_HOVER;
	m_enumMap["ACTION_ON_HOVER_OUT"] = ACTION_ON_HOVER_OUT;
	m_enumMap["ACTION_ON_CLICK"] = ACTION_ON_CLICK;
	m_enumMap["ACTION_SPRINT"] = ACTION_SPRINT;
	m_enumMap["ACTION_USAGE_SPECIAL"] = ACTION_USAGE_SPECIAL;

	//Keys
	m_enumMap["SDLK_ESCAPE"] = SDLK_ESCAPE;
	m_enumMap["SDLK_F1"] = SDLK_F1;
	m_enumMap["SDLK_F2"] = SDLK_F2;
	m_enumMap["SDLK_F3"] = SDLK_F3;
	m_enumMap["SDLK_F4"] = SDLK_F4;
	m_enumMap["SDLK_F12"] = SDLK_F12;

	//Scene Action Codes
	m_enumMap["SCENE_ACTION_QUIT"] = SCENE_ACTION_QUIT;
	m_enumMap["SCENE_ACTION_ADD"] = SCENE_ACTION_ADD;
	m_enumMap["SCENE_ACTION_ADD_AND_PAUSE"] = SCENE_ACTION_ADD_AND_PAUSE;
	m_enumMap["SCENE_ACTION_REPLACE"] = SCENE_ACTION_REPLACE;
	m_enumMap["SCENE_ACTION_LOAD_LEVEL"] = SCENE_ACTION_LOAD_LEVEL;
	m_enumMap["SCENE_ACTION_LOAD_NEXTLEVEL"] = SCENE_ACTION_LOAD_NEXTLEVEL;
	m_enumMap["SCENE_ACTION_EXIT"] = SCENE_ACTION_EXIT;
	m_enumMap["SCENE_ACTION_DIRECT"] = SCENE_ACTION_DIRECT;
	m_enumMap["SCENE_ACTION_RELEASE_DIRECT"] = SCENE_ACTION_RELEASE_DIRECT;
	m_enumMap["SCENE_ACTION_TOGGLE_SETTING"] = SCENE_ACTION_TOGGLE_SETTING;

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
	m_enumMap["GameLayer::BACKGROUND_1"] = GameLayer::BACKGROUND_1;
	m_enumMap["GameLayer::BACKGROUND_2"] = GameLayer::BACKGROUND_2;
	m_enumMap["GameLayer::MAIN"] = GameLayer::MAIN;
	m_enumMap["GameLayer::FOREGROUND_1"] = GameLayer::FOREGROUND_1;
	m_enumMap["GameLayer::FOREGROUND_2"] = GameLayer::FOREGROUND_2;
	m_enumMap["GameLayer::GUI"] = GameLayer::GUI;
	m_enumMap["GameLayer::ABSTRACT"] = GameLayer::ABSTRACT;
	m_enumMap["GameLayer::GRID_DISPLAY"] = GameLayer::GRID_DISPLAY;

	//Game Object Display Modes
	m_enumMap["DISPLAY_UI_MODE_STANDARD"] = DISPLAY_UI_MODE_STANDARD;
	m_enumMap["DISPLAY_UI_MODE_ONHOVER"] = DISPLAY_UI_MODE_ONHOVER;
	m_enumMap["DISPLAY_UI_MODE_ONCLICK"] = DISPLAY_UI_MODE_ONCLICK;

	//Particle Emitter types
	m_enumMap["ParticleEmitterType::ONETIME"] = ParticleEmitterType::ONETIME;
	m_enumMap["ParticleEmitterType::CONTINUOUS"] = ParticleEmitterType::CONTINUOUS;

	//OpenGL Texture Index values
	m_enumMap["GL_TextureIndexType::MAIN_TEXTURE_ATLAS"] = (int)GL_TextureIndexType::MAIN_TEXTURE_ATLAS;
	m_enumMap["GL_TextureIndexType::IMGUI_TEXTURE_ATLAS"] = (int)GL_TextureIndexType::IMGUI_TEXTURE_ATLAS;
	
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
	m_enumMap["waypoint"] = TraitTag::waypoint;
	m_enumMap["abstract"] = TraitTag::abstract;
	m_enumMap["interactive"] = TraitTag::interactive;
	m_enumMap["debug"] = TraitTag::debug;
	m_enumMap["pooled"] = TraitTag::pooled;
	m_enumMap["fragment"] = TraitTag::fragment;
	m_enumMap["objective"] = TraitTag::objective;
	m_enumMap["impasse"] = TraitTag::impasse;
	m_enumMap["conditional_impasse"] = TraitTag::conditional_impasse;
	m_enumMap["complex_impasse"] = TraitTag::complex_impasse;
	m_enumMap["mobile"] = TraitTag::mobile;

	//Hud Item Types
	m_enumMap["HudItemTypes::STATUS_SINGLE"] = (int)HudItemTypes::STATUS_SINGLE;
	m_enumMap["HudItemTypes::STATUS_SERIES"] = (int)HudItemTypes::STATUS_SERIES;

	m_enumMap["DISABLED_TYPE::RENDER"] = DISABLED_TYPE::RENDER;
	m_enumMap["DISABLED_TYPE::UPDATE"] = DISABLED_TYPE::UPDATE;
	m_enumMap["DISABLED_TYPE::PHYSICS"] = DISABLED_TYPE::PHYSICS;
	m_enumMap["DISABLED_TYPE::RENDER_AND_PHYSICS"] = DISABLED_TYPE::RENDER_AND_PHYSICS;
	m_enumMap["DISABLED_TYPE::RENDER_AND_UPDATE"] = DISABLED_TYPE::RENDER_AND_UPDATE;
	m_enumMap["DISABLED_TYPE::PHYICS_AND_UPDATE"] = DISABLED_TYPE::PHYICS_AND_UPDATE;

	//Scene Settings
	m_enumMap["DebugSceneSettings::SHOW_PHYSICS_DEBUG"] = DebugSceneSettings::SHOW_PHYSICS_DEBUG;
	m_enumMap["DebugSceneSettings::SHOW_NAVIGATION_DEBUG_MAP"] = DebugSceneSettings::SHOW_NAVIGATION_DEBUG_MAP;


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


