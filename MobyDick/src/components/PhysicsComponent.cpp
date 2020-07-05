#include "PhysicsComponent.h"

#include <memory>

#include "../GameObjectManager.h"
#include "../Globals.h"
#include "../GameObject.h"
#include "../EnumMaps.h"
#include "../GameConfig.h"
#include "../Game.h"

PhysicsComponent::PhysicsComponent()
{

}

PhysicsComponent::PhysicsComponent(std::string gameObjectId, std::shared_ptr<GameObject> parentGameObject, float xMapPos, float yMapPos, float angleAdjust)
{
	Json::Value itrJSON = GameObjectManager::instance().getDefinition(gameObjectId)->definitionJSON();

	//Save the pointer to parent GameObject
	m_parentGameObject = parentGameObject;

	//Pyhisics Component
	if (itrJSON.isMember("physicsComponent") && itrJSON.isMember("transformComponent"))
	{
		m_parentGameObject = parentGameObject;
		m_parentGameObject->setComponentFlag(PHYSICS_COMPONENT);

		//Get reference to the animationComponent JSON config and transformComponent JSON config
		Json::Value physicsComponentJSON = itrJSON["physicsComponent"];
		Json::Value transformComponentJSON = itrJSON["transformComponent"];

		m_physicsType = EnumMap::instance().toEnum(physicsComponentJSON["type"].asString());
		m_collisionShape = EnumMap::instance().toEnum(physicsComponentJSON["collisionShape"].asString());
		m_collisionRadius = physicsComponentJSON["collisionRadius"].asFloat();
		m_friction = physicsComponentJSON["friction"].asFloat();
		m_density = physicsComponentJSON["density"].asFloat();
		m_linearDamping = physicsComponentJSON["linearDamping"].asFloat();
		m_angularDamping = physicsComponentJSON["angularDamping"].asFloat();
		m_collisionCategory = EnumMap::instance().toEnum(physicsComponentJSON["collisionCategory"].asString());

		//Build the physics body
		m_physicsBody = buildB2Body(transformComponentJSON);

		//Calculate the spawn position
		//Translate the pixel oriented position into box2d meter-oriented
		b2Vec2* position = new b2Vec2
		(  (xMapPos * 32 + (transformComponentJSON["size"]["width"].asFloat() / 2)) / GameConfig::instance().scaleFactor(),
		   (yMapPos * 32 + (transformComponentJSON["size"]["width"].asFloat() / 2)) / GameConfig::instance().scaleFactor());

		//Calculate the spawn Angle
		float newAngle = util::degreesToRadians(angleAdjust);

		//Initial spawn position
		//FIXME:Need to pass in position info
		m_physicsBody->SetTransform(*position, newAngle);

	}

}

PhysicsComponent::~PhysicsComponent()
{

}

void PhysicsComponent::update()
{
	//Transfer the physicsComponent coordinates to the transformComponent
	m_parentGameObject->transformComponent().setPosition(
			m_physicsBody->GetPosition().x, 
			m_physicsBody->GetPosition().y, 
			m_physicsBody->GetAngle()
	);

}

b2Body* PhysicsComponent::buildB2Body(Json::Value transformComponentJSON)
{
	b2BodyDef bodyDef;

	bodyDef.type = static_cast<b2BodyType>(m_physicsType);

	//Default the position to zero.
	bodyDef.position.SetZero();
	b2Body* body = Game::instance().physicsWorld()->CreateBody(&bodyDef);

	b2Shape* shape;
	b2PolygonShape box;
	b2CircleShape circle;
	b2ChainShape chain;

	//Collision shape - will default to a rectangle
	if (m_collisionShape == b2Shape::e_circle)
	{
		circle.m_radius = m_collisionRadius;
		shape = &circle;
	}
	else if (m_collisionShape == b2Shape::e_chain)
	{
		//temp test code
		/*
		b2Vec2 vs[4];
		vs[0].Set(40, 40);
		vs[1].Set(400,400);
		vs[2].Set(50,500);
		chain.CreateChain(vs, 3);
		*/
		shape = &chain;
	}
	else
	{
		//Box Shape
		//Divide by 2 because box2d needs center position
		box.SetAsBox(
			transformComponentJSON["size"]["width"].asFloat() / GameConfig::instance().scaleFactor() / 2,
			transformComponentJSON["size"]["height"].asFloat() / GameConfig::instance().scaleFactor() / 2
		);
		shape = &box;
	}

	// Define the body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = shape;

	// Misc properties
	fixtureDef.density = m_density;
	fixtureDef.friction = m_friction;
	fixtureDef.restitution = m_restitution;

	//collision category
	fixtureDef.filter.categoryBits = m_collisionCategory;
	uint16 mask = this->setCollisionMask(m_collisionCategory);
	fixtureDef.filter.maskBits = mask;

	// Add the shape to the body.
	body->CreateFixture(&fixtureDef);

	body->SetLinearDamping(m_linearDamping);
	body->SetAngularDamping(m_angularDamping);

	body->SetUserData(m_parentGameObject.get());
	//this->box2dBodyCount++;
	return body;

}

uint16 PhysicsComponent::setCollisionMask(uint16 category)
{
	uint16 mask = 0;


	switch (category) {
	case COLLISION_PLAYER:
		mask = COLLISION_WALL | COLLISION_PARTICLE2 | COLLISION_PARTICLE3 | COLLISION_ENEMY_FRAME
			| COLLISION_ENEMY_ARMOR_PIECE;
		break;
	case COLLISION_WALL:
		mask = COLLISION_PLAYER | COLLISION_PARTICLE1 | COLLISION_PARTICLE2 | COLLISION_PARTICLE3
			| COLLISION_ENEMY_FRAME | COLLISION_PLAYER_BULLET | COLLISION_ENEMY_ARMOR_PIECE;
		break;
	case COLLISION_PLAYER_BULLET:
		mask = COLLISION_WALL | COLLISION_ENEMY_ARMOR;
		break;
	case COLLISION_PARTICLE1:
		mask = COLLISION_WALL | COLLISION_PLAYER;
		break;
	case COLLISION_PARTICLE2:
		mask = COLLISION_WALL | COLLISION_PLAYER;
		break;
	case COLLISION_PARTICLE3:
		mask = COLLISION_WALL | COLLISION_PLAYER;
		break;
	case COLLISION_ENEMY_FRAME:
		mask = COLLISION_WALL | COLLISION_PLAYER | COLLISION_ENEMY_ARMOR_PIECE;
		break;
	case COLLISION_ENEMY_ARMOR:
		mask = COLLISION_PLAYER_BULLET;
		break;
	case COLLISION_ENEMY_ARMOR_PIECE:
		mask = COLLISION_PLAYER | COLLISION_WALL | COLLISION_ENEMY_FRAME | COLLISION_ENEMY_ARMOR_PIECE;
		break;


	}

	return mask;

}


