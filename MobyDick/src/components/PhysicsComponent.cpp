#include "PhysicsComponent.h"

#include <memory>

#include "../GameObjectManager.h"
#include "../Globals.h"
#include "../GameObject.h"
#include "../EnumMaps.h"
#include "../Game.h"

PhysicsComponent::PhysicsComponent()
{

}

PhysicsComponent::PhysicsComponent(std::string gameObjectId, std::shared_ptr<GameObject> parentGameObject)
{
	Json::Value itrJSON = GameObjectManager::instance().getDefinition(gameObjectId)->definitionJSON();

	//Save the pointer to parent GameObject
	m_parentGameObject = parentGameObject;

	//Pyhisics Component
	if (itrJSON.isMember("physicsComponent"))
	{
		m_parentGameObject = parentGameObject;
		m_parentGameObject->setComponentFlag(PHYSICS_COMPONENT);

		//Convenvience reference
		Json::Value itr = itrJSON["physicsComponent"];

		m_physicsType = EnumMap::instance().toEnum(itr["type"].asString());
		m_collisionShape = EnumMap::instance().toEnum(itr["collisionShape"].asString());
		m_collisionRadius = itr["collisionRadius"].asFloat();
		m_friction = itr["friction"].asFloat();
		m_density = itr["density"].asFloat();
		m_linearDamping = itr["linearDamping"].asFloat();
		m_angularDamping = itr["angularDamping"].asFloat();
		m_collisionCategory = EnumMap::instance().toEnum(itr["collisionCategory"].asString());

		//Build the physics body
		m_physicsBody = buildB2Body();

		//Initial spawn position
		//FIXME:Need to pass in position info
		m_physicsBody->SetTransform(b2Vec2(2, 2), 0);





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

b2Body* PhysicsComponent::buildB2Body()
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
		box.SetAsBox(
			m_parentGameObject->transformComponent().size().x / 2, 
			m_parentGameObject->transformComponent().size().y / 2);
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


