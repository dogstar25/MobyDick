#include "GameObjectContactListener.h"
#include "GameObject.h"
#include "WorldObject.h"
#include "game.h"


// this callback will find the closest static body that the ray hits
class FirstHitGroundRayCastCallback : public b2RayCastCallback
{
public:
	b2Vec2 m_point;
	b2Fixture* m_fixture;

	FirstHitGroundRayCastCallback()
	{
		m_fixture = NULL;
	}

	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
	{
		//ignore non-static bodies
		if (fixture->GetBody()->GetType() != b2_staticBody)
			return -1;

		m_point = point;
		m_fixture = fixture;
		return fraction;
	}
};

void GameObjectContactListener::BeginContact(b2Contact* contact) {

	//check if fixture A was a ball
	WorldObject* gameObjectA = static_cast<WorldObject*>(contact->GetFixtureA()->GetBody()->GetUserData());
	if (gameObjectA)
	{
		//static_cast<GameObject*>(bodyUserData)->startContact();
	}

	//check if fixture B was a ball
	WorldObject* gameObjectB = static_cast<WorldObject*>(contact->GetFixtureB()->GetBody()->GetUserData());
	if (gameObjectB)
	{
		//static_cast<GameObject*>(bodyUserData)->startContact();
		//gameObjectB->physicsBody->SetActive(false);
		//gameObjectB->physicsBody->SetAwake(false);
		
	}

	float32 x = 0;
	float32 y = 0;
	b2WorldManifold worldManifold;

	contact->GetWorldManifold(&worldManifold);
	b2Vec2 contactPoint = worldManifold.points[0];

	if (gameObjectA != NULL && gameObjectB != NULL)
	{
		handleContact(gameObjectA, gameObjectB, contactPoint);
	}

}


void GameObjectContactListener::EndContact(b2Contact* contact)
{

	//check if fixture A was a ball
	WorldObject* gameObjectA = static_cast<WorldObject*>(contact->GetFixtureA()->GetBody()->GetUserData());
	if (gameObjectA)
	{
		//static_cast<GameObject*>(bodyUserData)->startContact();
	}

	//check if fixture B was a ball
	WorldObject* gameObjectB = static_cast<WorldObject*>(contact->GetFixtureB()->GetBody()->GetUserData());
	if (gameObjectB)
	{
		//static_cast<GameObject*>(bodyUserData)->startContact();
	}

}



void GameObjectContactListener::handleContact(WorldObject* contact1, WorldObject* contact2, b2Vec2 contactPoint)
{
	WorldObject* bullet=NULL, *wall = NULL, *player = NULL, *piece = NULL;

	/*
		TODO. Create a Designation or Category variable to use to compare objects for contact
				The id name is too arbitrary
	
	*/
	if (contact1->definition->id.compare("BULLET1") == 0 ||
		contact2->definition->id.compare("BULLET1") == 0)
	{
		//Bullet Wall contact
		if (contact1->definition->id.compare(0,4,"WALL") == 0 ||
			contact2->definition->id.compare(0, 4, "WALL") == 0)
		{
			if (contact1->definition->id.compare("BULLET1") == 0) {
				bullet = contact1;
				wall = contact2;
			}
			else if (contact2->definition->id.compare("BULLET1") == 0) {
				bullet = contact2;
				wall = contact1;
			}

			this->bulletWall(bullet, wall, contactPoint);

		}
		//Bullet Piece contact
		if (contact1->definition->id.compare(5, 5, "PIECE") == 0 ||
			contact1->definition->id.compare(7, 5, "PIECE") == 0 ||
			contact2->definition->id.compare(5, 5, "PIECE") == 0 ||
			contact2->definition->id.compare(7, 5, "PIECE") == 0)
		{
			if (contact1->definition->id.compare("BULLET1") == 0) {
				bullet = contact1;
				piece = contact2;
			}
			else if (contact2->definition->id.compare("BULLET1") == 0) {
				bullet = contact2;
				piece = contact1;
			}

			this->bulletPiece(bullet, piece, contactPoint);

		}


	}
	if (contact1->definition->id.compare("GINA_64") == 0 ||
		contact2->definition->id.compare("GINA_64") == 0)
	{
		//Bullet Wall contact
		if (contact1->definition->id.compare("ANGLE_BIT_PIECE") == 0 ||
			contact2->definition->id.compare("ANGLE_BIT_PIECE") == 0)
		{
			if (contact1->definition->id.compare("GINA_64") == 0) {
				player = contact1;
				piece = contact2;
			}
			else if (contact2->definition->id.compare("GINA_64") == 0) {
				player = contact2;
				piece = contact1;
			}

			this->playerBitPiece(player, piece, contactPoint);

		}
	}


}

void GameObjectContactListener::playerBitPiece(WorldObject* player, WorldObject* piece, b2Vec2 contactPoint)
{
	//Set flag for bullet to be removed from world
	piece->removeFromWorld = true;

}


void GameObjectContactListener::bulletWall(WorldObject* bullet, WorldObject* wall, b2Vec2 contactPoint)
{
	//Set flag for bullet to be removed from world
	bullet->removeFromWorld = true;

	//use the collision point for the particle emission
	float x = contactPoint.x;
	float y = contactPoint.y;
	b2Vec2 particleOrigin = { x,y };
	//This position might be "inside" of the wall object
	//We will do a ray trace from this position towards the play object untl it is no long inside the wall
	//findWallImpactPoint(worldPoint, game->player.get());

	//temp color code
	SDL_Color colorMin = { 0,0,0,255 };
	SDL_Color colorMax = { 225,255,255,255 };


	ParticleEmission* particleEmission = new ParticleEmission(
		"PARTICLE1_POOL",
		particleOrigin, //min position
		particleOrigin,	//max position
		5,	//Force Min
		5,	//force Max
		0.55,	//Lifetime Min
		1.55,	//Lifetime Max
		true,	// Alpha fade
		0,	//Angle min
		360,	//Angle Max
		0.28,	//Size Min
		1.48,	//Size Max
		colorMin,	//Color Min
		colorMax,	//Color Max
		40,	//Particle count min
		50	//Particle count max
	);
	game->particleMachine.add(particleEmission);

}

void GameObjectContactListener::bulletPiece(WorldObject* bullet, WorldObject* piece, b2Vec2 contactPoint)
{

	ParticleEmission* particleEmission = NULL;

	//Set flag for bullet to be removed from world
	bullet->removeFromWorld = true;

	//Set flag for piece to be removed from world
	piece->removeFromWorld = true;

	//use the collision point for the particle emission
	float x = contactPoint.x;
	float y = contactPoint.y;
	b2Vec2 particleOrigin = {x,y};

	//temp color code
	SDL_Color colorMin = piece->color;
	SDL_Color colorMax = piece->color;

	particleEmission = new ParticleEmission(
		"PARTICLE1_POOL",
		particleOrigin, //min position
		particleOrigin,	//max position
		5,	//Force Min
		15,	//force Max
		0.55,	//Lifetime Min
		1.55,	//Lifetime Max
		true,	// Alpha fade
		0,	//Angle min
		360,	//Angle Max
		0.28,	//Size Min
		1.48,	//Size Max
		colorMin,	//Color Min
		colorMax,	//Color Max
		5,	//Particle count min
		12	//Particle count max
	);
	game->particleMachine.add(particleEmission);
	

	//Also emit 2 pieces that will remain on teh ground
	particleEmission = new ParticleEmission(
		"PIECES1_POOL",
		particleOrigin, //min position
		particleOrigin,	//max position
		20,	//Force Min
		50,	//force Max
		0,	//Lifetime Min
		0,	//Lifetime Max
		false,	// Alpha fade
		0,	//Angle min
		360,	//Angle Max
		0.16,	//Size Min
		0.16,	//Size Max
		colorMin,	//Color Min
		colorMax,	//Color Max
		2,	//Particle count min
		2	//Particle count max
	);
	game->particleMachine.add(particleEmission);
}

b2Vec2 GameObjectContactListener::findWallImpactPoint(b2Vec2 worldPoint, PlayerObject* player)
{

	b2Vec2 spawnPoint;

	FirstHitGroundRayCastCallback raycastCallback;
	game->physicsWorld->RayCast(&raycastCallback, worldPoint, player->physicsBody->GetTransform().p);



	return spawnPoint;



}




