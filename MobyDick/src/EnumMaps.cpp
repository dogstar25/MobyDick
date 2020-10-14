#include "EnumMaps.h"

#include <box2d/box2d.h>

EnumMap& EnumMap::instance()
{

	static EnumMap singletonInstance;
	return singletonInstance;

}
EnumMap::EnumMap()
{

	//Mouse State
	m_enumMap["MOUSE_NONE"] = MOUSE_NONE;
	m_enumMap["MOUSE_HOVER"] = MOUSE_HOVER;
	m_enumMap["MOUSE_NONE"] = MOUSE_HOLD;
	m_enumMap["MOUSE_HOVER"] = MOUSE_CLICKED;

	//Mouse Modes
	m_enumMap["CONTROL_MODE_SELECT"] = CONTROL_MODE_SELECT;
	m_enumMap["CONTROL_MODE_PLAY"] = CONTROL_MODE_PLAY;

	m_enumMap["IdTag::GENERAL"] = IdTag::GENERAL;
	m_enumMap["IdTag::DEFLECT_EFFECT"] = IdTag::DEFLECT_EFFECT;
	m_enumMap["IdTag::SMOKE_PARTICLE"] = IdTag::SMOKE_PARTICLE;
	m_enumMap["IdTag::DRONE_BRAIN"] = IdTag::DRONE_BRAIN;
	m_enumMap["IdTag::DRONE_SHIELD"] = IdTag::DRONE_SHIELD;
	m_enumMap["IdTag::DRONE_WEAPON"] = IdTag::DRONE_WEAPON;
	m_enumMap["IdTag::DRONE_FRAME"] = IdTag::DRONE_FRAME;
	m_enumMap["IdTag::ENEMY_BULLET"] = IdTag::ENEMY_BULLET;
	m_enumMap["IdTag::FRIENDLY_BULLET"] = IdTag::FRIENDLY_BULLET;
	m_enumMap["IdTag::GUI"] = IdTag::GUI;
	m_enumMap["IdTag::HEAVY_PARTICLE"] = IdTag::HEAVY_PARTICLE;
	m_enumMap["IdTag::LIGHT_PARTICLE"] = IdTag::LIGHT_PARTICLE;
	m_enumMap["IdTag::PLAYER"] = IdTag::PLAYER;
	m_enumMap["IdTag::PLAYER_BULLET"] = IdTag::PLAYER_BULLET;
	m_enumMap["IdTag::SHIELD_SCRAP"] = IdTag::SHIELD_SCRAP;
	m_enumMap["IdTag::SURVIVOR"] = IdTag::SURVIVOR;
	m_enumMap["IdTag::MEDKIT"] = IdTag::MEDKIT;
	m_enumMap["IdTag::WALL"] = IdTag::WALL;
	m_enumMap["IdTag::WEAPON_PICKUP"] = IdTag::WEAPON_PICKUP;

	//Box2d Related
	m_enumMap["B2_STATIC"] = b2_staticBody;
	m_enumMap["B2_KINEMATIC"] = b2_kinematicBody;
	m_enumMap["B2_DYNAMIC"] = b2_dynamicBody;
	m_enumMap["B2_CIRCLE"] = b2Shape::e_circle;
	m_enumMap["B2_BOX"] = b2Shape::e_polygon;
	m_enumMap["B2_CHAIN"] = b2Shape::e_chain;

	//Animation States
	m_enumMap["ANIMATION_IDLE"] = ANIMATION_IDLE;
	m_enumMap["ANIMATION_RUN"] = ANIMATION_RUN;
	m_enumMap["ANIMATION_ACTIVE"] = ANIMATION_ACTIVE;
	m_enumMap["ANIMATION_ACTION"] = ANIMATION_ACTION;

	//Component Types
	/*m_enumMap["TRANSFORM_COMPONENT"] = TRANSFORM_COMPONENT;
	m_enumMap["ANIMATION_COMPONENT"] = ANIMATION_COMPONENT;
	m_enumMap["CHILDREN_COMPONENT"] = CHILDREN_COMPONENT;
	m_enumMap["RENDER_COMPONENT"] = RENDER_COMPONENT;
	m_enumMap["PHYSICS_COMPONENT"] = PHYSICS_COMPONENT;
	m_enumMap["VITALITY_COMPONENT"] = VITALITY_COMPONENT;
	m_enumMap["WEAPON_COMPONENT"] = WEAPON_COMPONENT;
	m_enumMap["TEXT_COMPONENT"] = TEXT_COMPONENT;
	m_enumMap["COMPOSITE_COMPONENT"] = COMPOSITE_COMPONENT;
	m_enumMap["PARTICLE_COMPONENT"] = PARTICLE_COMPONENT;
	m_enumMap["ATTACHMENTS_COMPONENT"] = ATTACHMENTS_COMPONENT;
	m_enumMap["PLAYERCONTROL_COMPONENT"] = PLAYERCONTROL_COMPONENT;
	m_enumMap["INVENTORY_COMPONENT"] = INVENTORY_COMPONENT;*/

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
	
	//Game Layers
	m_enumMap["LAYER_BACKGROUND"] = LAYER_BACKGROUND;
	m_enumMap["LAYER_MAIN"] = LAYER_MAIN;
	m_enumMap["LAYER_TEXT"] = LAYER_TEXT;

	//Game Object Display Modes
	m_enumMap["DISPLAY_UI_MODE_STANDARD"] = DISPLAY_UI_MODE_STANDARD;
	m_enumMap["DISPLAY_UI_MODE_ONHOVER"] = DISPLAY_UI_MODE_ONHOVER;
	m_enumMap["DISPLAY_UI_MODE_ONCLICK"] = DISPLAY_UI_MODE_ONCLICK;

	//Global Screen Position
	m_enumMap["WindowPosition::CENTER"] = WindowPosition::CENTER;

	//Particle Emitter types
	m_enumMap["ParticleEmitterType::ONETIME"] = ParticleEmitterType::ONETIME;
	m_enumMap["ParticleEmitterType::CONTINUOUS"] = ParticleEmitterType::CONTINUOUS;

	////Particle Effects
	//m_enumMap["ParticleEffects::ricochet"] = ParticleEffects::ricochet;
	//m_enumMap["ParticleEffects::scrap"] = ParticleEffects::scrap;

	//Texure Blend Modes
	m_enumMap["SDL_BLENDMODE_BLEND"] = SDL_BLENDMODE_BLEND;
	m_enumMap["SDL_BLENDMODE_ADD"] = SDL_BLENDMODE_ADD;
	

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


