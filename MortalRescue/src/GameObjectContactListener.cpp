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
	b2Vec2 worldPoint = worldManifold.points[0];

	if (gameObjectA != NULL && gameObjectB != NULL)
	{
		if (gameObjectA->definition->id == "BULLET1" ||
			gameObjectB->definition->id == "BULLET1")
		{
			if (gameObjectA->definition->id == "BULLET1") {
				gameObjectA->removeFromWorld = true;

			}
			if (gameObjectB->definition->id == "BULLET1") {
				gameObjectB->removeFromWorld = true;

			}

			//use the collision point for the particle emission
			x = worldPoint.x;
			y = worldPoint.y;

			//This position might be "inside" of the wall object
			//We will do a ray trace from this position towards the play object untl it is no long inside the wall
			//findWallImpactPoint(worldPoint, game->player.get());

			//temp color code
			SDL_Color colorMin = { 1,1,1,255 };
			SDL_Color colorMax = { 225,255,255,255 };

			
			ParticleEmission* particleEmission = new ParticleEmission(
				"PARTICLE1_POOL",
				x,	// X position
				y,	//Y Position
				5,	//Force Min
				5,	//force Max
				0.1,	//Lifetime Min
				0.1,	//Lifetime Max
				true,	// Alpha fade
				0,	//Angle min
				360,	//Angle Max
				0.28,	//Size Min
				0.28,	//Size Max
				colorMin,	//Color Min
				colorMax,	//Color Max
				6,	//Particle count min
				6	//Particle count max
			);
			game->particleMachine.add(particleEmission);

		}
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

b2Vec2 GameObjectContactListener::findWallImpactPoint(b2Vec2 worldPoint, PlayerObject* player)
{

	b2Vec2 spawnPoint;

	FirstHitGroundRayCastCallback raycastCallback;
	game->physicsWorld->RayCast(&raycastCallback, worldPoint, player->physicsBody->GetTransform().p);



	return spawnPoint;



}




