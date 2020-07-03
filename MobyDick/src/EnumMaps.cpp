#include "EnumMaps.h"

#include "box2d/box2d.h"

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

	//Collision Groups
	m_enumMap["COLLISION_GENERIC"] = COLLISION_GENERIC;
	m_enumMap["COLLISION_PLAYER"] = COLLISION_PLAYER;
	m_enumMap["COLLISION_WALL"] = COLLISION_WALL;
	m_enumMap["COLLISION_PLAYER_BULLET"] = COLLISION_PLAYER_BULLET;
	m_enumMap["COLLISION_PARTICLE1"] = COLLISION_PARTICLE1;
	m_enumMap["COLLISION_PARTICLE2"] = COLLISION_PARTICLE2;
	m_enumMap["COLLISION_PARTICLE3"] = COLLISION_PARTICLE3;
	m_enumMap["COLLISION_ENEMY_FRAME"] = COLLISION_ENEMY_FRAME;
	m_enumMap["COLLISION_ENEMY_ARMOR"] = COLLISION_ENEMY_ARMOR;
	m_enumMap["COLLISION_ENEMY_ARMOR_PIECE"] = COLLISION_ENEMY_ARMOR_PIECE;

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
	m_enumMap["TRANSFORM_COMPONENT"] = TRANSFORM_COMPONENT;
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

}

EnumMap::~EnumMap()
{
	m_enumMap.clear();

}

size_t EnumMap::toEnum(std::string name)
{

	auto iter = m_enumMap.find(name);

	if (iter != m_enumMap.end())
	{
		return m_enumMap[name];
	}
	else
	{
		//FIXME: this could be problematic. Need to consider throwing an error. A zero being returned can be a valid enum
		//a -1 might get used in a bitwise operation and make a hard to find bug
		return(-1);
	}

	

}


