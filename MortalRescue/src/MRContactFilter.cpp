#include "MRContactFilter.h"

#include "GameConstants.h"
#include "GameObject.h"

MRContactFilter::MRContactFilter()
{

	using namespace ContactTag;

	//LEVEL_CAGE
	m_contactMasks[LEVEL_CAGE].reset();
	m_contactMasks[LEVEL_CAGE].set(PLAYER_COLLISION);
	m_contactMasks[LEVEL_CAGE].set(PLAYER_BULLET);
	m_contactMasks[LEVEL_CAGE].set(DRONE_FRAME);
	m_contactMasks[LEVEL_CAGE].set(SHIELD_SCRAP);
	m_contactMasks[LEVEL_CAGE].set(SURVIVOR);
	m_contactMasks[LEVEL_CAGE].set(DRONE_SCRAP);

	//Intangible - set all to intangle
	m_contactMasks[DEFLECT_EFFECT].reset();

	//DRONE_BRAIN
	m_contactMasks[DRONE_BRAIN].reset();
	m_contactMasks[DRONE_BRAIN].set(WALL);
	m_contactMasks[DRONE_BRAIN].set(PLAYER_BULLET);

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
	m_contactMasks[DRONE_SHIELD].set(PLAYER_BULLET);
	m_contactMasks[DRONE_SHIELD].set(PLAYER_PULSE_BULLET);
	m_contactMasks[DRONE_SHIELD].set(PLAYER_PULSE_EXPLOSION);

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
	m_contactMasks[HEAVY_PARTICLE].set(DRONE_FRAME);
	m_contactMasks[HEAVY_PARTICLE].set(PLAYER_HITBOX);
	m_contactMasks[HEAVY_PARTICLE].set(DRONE_SHIELD);
	m_contactMasks[HEAVY_PARTICLE].set(DRONE_SCRAP);

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
	m_contactMasks[ENEMY_BULLET].set(ENEMY_TURRET);
	m_contactMasks[ENEMY_BULLET].set(WALL);

	//PLAYER_COLLISION
	m_contactMasks[PLAYER_COLLISION].reset();
	m_contactMasks[PLAYER_COLLISION].set(GENERAL_SOLID);
	m_contactMasks[PLAYER_COLLISION].set(SHIELD_SCRAP);
	m_contactMasks[PLAYER_COLLISION].set(SURVIVOR);
	m_contactMasks[PLAYER_COLLISION].set(DRONE_FRAME);
	m_contactMasks[PLAYER_COLLISION].set(WALL);
	m_contactMasks[PLAYER_COLLISION].set(WALL_PIECE);
	m_contactMasks[PLAYER_COLLISION].set(RADIATION_PARTICLE);
	m_contactMasks[PLAYER_COLLISION].set(DRONE_SCRAP);
	m_contactMasks[PLAYER_COLLISION].set(HEART_PICKUP);
	m_contactMasks[PLAYER_COLLISION].set(LEVEL_CAGE);

	//PLAYER_TOUCH
	m_contactMasks[PLAYER_TOUCH].reset();
	m_contactMasks[PLAYER_TOUCH].flip();
	m_contactMasks[PLAYER_TOUCH].set(SURVIVOR);

	//PLAYER_HITBOX
	m_contactMasks[PLAYER_HITBOX].reset();
	m_contactMasks[PLAYER_HITBOX].set(ENEMY_BULLET);
	m_contactMasks[PLAYER_HITBOX].set(ENEMY_TURRET);
	m_contactMasks[PLAYER_HITBOX].set(HEAVY_PARTICLE);
	m_contactMasks[PLAYER_HITBOX].set(RADIATION_PARTICLE);

	//PLAYER_BULLET
	m_contactMasks[PLAYER_BULLET].reset();
	m_contactMasks[PLAYER_BULLET].set(DRONE_SHIELD);
	m_contactMasks[PLAYER_BULLET].set(DRONE_BRAIN);
	m_contactMasks[PLAYER_BULLET].set(WALL);
	m_contactMasks[PLAYER_BULLET].set(WALL_PIECE);
	m_contactMasks[PLAYER_BULLET].set(ENEMY_TURRET);
	m_contactMasks[PLAYER_BULLET].set(HEALTH_STATION_INSIDE);
	m_contactMasks[PLAYER_BULLET].set(LEVEL_CAGE);

	//PLAYER_PULSE_BULLET
	m_contactMasks[PLAYER_PULSE_BULLET].reset();
	m_contactMasks[PLAYER_PULSE_BULLET].set(DRONE_SHIELD);
	m_contactMasks[PLAYER_PULSE_BULLET].set(WALL_PIECE);
	m_contactMasks[PLAYER_PULSE_BULLET].set(ENEMY_TURRET);
	m_contactMasks[PLAYER_PULSE_BULLET].set(HEALTH_STATION_INSIDE);

	//PLAYER_PULSE_EXPLOSION
	m_contactMasks[PLAYER_PULSE_EXPLOSION].reset();
	m_contactMasks[PLAYER_PULSE_EXPLOSION].set(DRONE_SHIELD);
	m_contactMasks[PLAYER_PULSE_EXPLOSION].set(WALL_PIECE);
	m_contactMasks[PLAYER_PULSE_EXPLOSION].set(ENEMY_TURRET);

	//SHIELD_SCRAP
	m_contactMasks[SHIELD_SCRAP].reset();
	m_contactMasks[SHIELD_SCRAP].set(PLAYER_COLLISION);
	m_contactMasks[SHIELD_SCRAP].set(DRONE_FRAME);
	m_contactMasks[SHIELD_SCRAP].set(ENEMY_TURRET);
	m_contactMasks[SHIELD_SCRAP].set(WALL);
	m_contactMasks[SHIELD_SCRAP].set(LEVEL_CAGE);

	//SURVIVOR
	m_contactMasks[SURVIVOR].reset();
	m_contactMasks[SURVIVOR].set(PLAYER_COLLISION);
	m_contactMasks[SURVIVOR].set(PLAYER_TOUCH);
	m_contactMasks[SURVIVOR].set(SURVIVOR);
	m_contactMasks[SURVIVOR].set(WALL);
	m_contactMasks[SURVIVOR].set(ESCAPE_STAIRS);
	m_contactMasks[SURVIVOR].set(ENEMY_TURRET);
	m_contactMasks[SURVIVOR].set(WALL_PIECE);
	m_contactMasks[SURVIVOR].set(LEVEL_CAGE);

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
	m_contactMasks[WALL].set(DRONE_SCRAP);
	m_contactMasks[WALL].set(RADIATION_PARTICLE);
	
	//BUTTONS
	m_contactMasks[BUTTON].reset();
	m_contactMasks[BUTTON].set(GENERAL_SOLID);
	m_contactMasks[BUTTON].set(PLAYER_TOUCH);

	//ESCAPE STAIRS
	m_contactMasks[ESCAPE_STAIRS].reset();
	m_contactMasks[ESCAPE_STAIRS].set(GENERAL_SOLID);
	m_contactMasks[ESCAPE_STAIRS].set(SURVIVOR);

	//Wall Piece
	m_contactMasks[WALL_PIECE].reset();
	m_contactMasks[WALL_PIECE].set(GENERAL_SOLID);
	m_contactMasks[WALL_PIECE].set(SURVIVOR);
	m_contactMasks[WALL_PIECE].set(PLAYER_COLLISION);
	m_contactMasks[WALL_PIECE].set(PLAYER_BULLET);
	m_contactMasks[WALL_PIECE].set(PLAYER_PULSE_BULLET);
	m_contactMasks[WALL_PIECE].set(PLAYER_PULSE_EXPLOSION);

	//Turret
	m_contactMasks[ENEMY_TURRET].reset();
	m_contactMasks[ENEMY_TURRET].set(SURVIVOR);
	m_contactMasks[ENEMY_TURRET].set(SHIELD_SCRAP);
	m_contactMasks[ENEMY_TURRET].set(PLAYER_BULLET);

	//Drone Scrap
	m_contactMasks[DRONE_SCRAP].reset();
	m_contactMasks[DRONE_SCRAP].set(HEAVY_PARTICLE);
	m_contactMasks[DRONE_SCRAP].set(PLAYER_COLLISION);
	m_contactMasks[DRONE_SCRAP].set(WALL);
	m_contactMasks[DRONE_SCRAP].set(LEVEL_CAGE);

	//Heart Pickup
	m_contactMasks[HEART_PICKUP].reset();
	m_contactMasks[HEART_PICKUP].set(PLAYER_COLLISION);
	m_contactMasks[HEART_PICKUP].set(HEALTH_STATION_INSIDE);

	//HEALTH_STATION_INSIDE Inside
	m_contactMasks[HEALTH_STATION_INSIDE].reset();
	m_contactMasks[HEALTH_STATION_INSIDE].set(HEART_PICKUP);
	m_contactMasks[HEALTH_STATION_INSIDE].set(PLAYER_BULLET);
	m_contactMasks[HEALTH_STATION_INSIDE].set(PLAYER_PULSE_BULLET);

	//Radiation Particle
	m_contactMasks[RADIATION_PARTICLE].reset();
	m_contactMasks[RADIATION_PARTICLE].set(WALL);
	m_contactMasks[RADIATION_PARTICLE].set(DRONE_FRAME);
	m_contactMasks[RADIATION_PARTICLE].set(PLAYER_HITBOX);
	m_contactMasks[RADIATION_PARTICLE].set(DRONE_SHIELD);
	m_contactMasks[RADIATION_PARTICLE].set(DRONE_SCRAP);
	

}

bool MRContactFilter::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
{
	//Call base function
	return ContactFilter::ShouldCollide(fixtureA, fixtureB);


}
