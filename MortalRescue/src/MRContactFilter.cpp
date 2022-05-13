#include "MRContactFilter.h"

#include "GameConstants.h"
#include "GameObject.h"

MRContactFilter::MRContactFilter()
{

	using namespace ContactTag;

	//Intangible - set all to intangle
	m_contactMasks[DEFLECT_EFFECT].reset();

	//DRONE_BRAIN
	m_contactMasks[DRONE_BRAIN].reset();
	m_contactMasks[DRONE_BRAIN].set(WALL);

	//DRONE_FRAME
	m_contactMasks[DRONE_FRAME].reset();
	m_contactMasks[DRONE_FRAME].set(HEAVY_PARTICLE);
	m_contactMasks[DRONE_FRAME].set(PLAYER_COLLISION);
	m_contactMasks[DRONE_FRAME].set(SHIELD_SCRAP);
	m_contactMasks[DRONE_FRAME].set(SURVIVOR);
	m_contactMasks[DRONE_FRAME].set(WALL);
	m_contactMasks[DRONE_FRAME].set(DRONE_FRAME);

	//DRONE_SHIELD
	m_contactMasks[DRONE_SHIELD].reset();
	m_contactMasks[DRONE_SHIELD].set(HEAVY_PARTICLE);
	m_contactMasks[DRONE_SHIELD].set(LIGHT_PARTICLE);
	m_contactMasks[DRONE_SHIELD].set(PLAYER_BULLET);

	//DRONE_WEAPON
	m_contactMasks[DRONE_WEAPON].reset();
	m_contactMasks[DRONE_WEAPON].set(FRIENDLY_BULLET);
	m_contactMasks[DRONE_WEAPON].set(HEAVY_PARTICLE);
	m_contactMasks[DRONE_WEAPON].set(PLAYER_COLLISION);
	m_contactMasks[DRONE_WEAPON].set(PLAYER_BULLET);
	m_contactMasks[DRONE_WEAPON].set(SURVIVOR);
	m_contactMasks[DRONE_WEAPON].set(WALL);

	//HEAVY_PARTICLE
	m_contactMasks[HEAVY_PARTICLE].reset();
	m_contactMasks[HEAVY_PARTICLE].set(WALL);

	//LIGHT_PARTICLE
	m_contactMasks[LIGHT_PARTICLE].reset();
	m_contactMasks[LIGHT_PARTICLE].set(WALL);

	//FRIENDLY_BULLET
	m_contactMasks[FRIENDLY_BULLET].reset();
	m_contactMasks[FRIENDLY_BULLET].set(WALL);

	//ENEMY_BULLET
	m_contactMasks[ENEMY_BULLET].reset();
	m_contactMasks[ENEMY_BULLET].set(PLAYER_HITBOX);
	m_contactMasks[ENEMY_BULLET].set(SURVIVOR);
	m_contactMasks[ENEMY_BULLET].set(WALL);

	//PLAYER_COLLISION
	m_contactMasks[PLAYER_COLLISION].reset();
	m_contactMasks[PLAYER_COLLISION].set(GENERAL_SOLID);
	m_contactMasks[PLAYER_COLLISION].set(SHIELD_SCRAP);
	m_contactMasks[PLAYER_COLLISION].set(SURVIVOR);
	m_contactMasks[PLAYER_COLLISION].set(DRONE_FRAME);
	m_contactMasks[PLAYER_COLLISION].set(WALL);

	//PLAYER_TOUCH
	m_contactMasks[PLAYER_TOUCH].reset();
	m_contactMasks[PLAYER_TOUCH].flip();
	m_contactMasks[PLAYER_TOUCH].set(SURVIVOR);
	

	//PLAYER_HITBOX
	m_contactMasks[PLAYER_HITBOX].reset();
	m_contactMasks[PLAYER_HITBOX].set(ENEMY_BULLET);

	//PLAYER_BULLET
	m_contactMasks[PLAYER_BULLET].reset();
	m_contactMasks[PLAYER_BULLET].set(DRONE_SHIELD);
	m_contactMasks[PLAYER_BULLET].set(WALL);

	//SHIELD_SCRAP
	m_contactMasks[SHIELD_SCRAP].reset();
	m_contactMasks[SHIELD_SCRAP].set(PLAYER_COLLISION);
	m_contactMasks[SHIELD_SCRAP].set(DRONE_FRAME);
	m_contactMasks[SHIELD_SCRAP].set(WALL);

	//SURVIVOR
	m_contactMasks[SURVIVOR].reset();
	m_contactMasks[SURVIVOR].set(PLAYER_COLLISION);
	m_contactMasks[SURVIVOR].set(PLAYER_TOUCH);
	m_contactMasks[SURVIVOR].set(SURVIVOR);
	m_contactMasks[SURVIVOR].set(WALL);

	//WALL
	m_contactMasks[WALL].reset();
	m_contactMasks[WALL].set(GENERAL_SOLID);
	m_contactMasks[WALL].set(HEAVY_PARTICLE);
	m_contactMasks[WALL].set(PLAYER_COLLISION);
	m_contactMasks[WALL].set(DRONE_FRAME);
	m_contactMasks[WALL].set(FRIENDLY_BULLET);
	m_contactMasks[WALL].set(PLAYER_BULLET);
	m_contactMasks[WALL].set(SHIELD_SCRAP);
	m_contactMasks[WALL].set(SURVIVOR);
	m_contactMasks[WALL].set(ENEMY_BULLET);

	//BUTTONS
	m_contactMasks[BUTTON].reset();
	m_contactMasks[BUTTON].set(GENERAL_SOLID);
	m_contactMasks[BUTTON].set(PLAYER_TOUCH);

}

bool MRContactFilter::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
{
	//Call base function
	return ContactFilter::ShouldCollide(fixtureA, fixtureB);


}
