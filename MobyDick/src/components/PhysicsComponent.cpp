#include "PhysicsComponent.h"

#include <memory>

#include "TransformComponent.h"
#include "../EntityDefinitionManager.h"
#include "../Globals.h"
#include "../GameObject.h"
#include "../EnumMaps.h"
#include "../GameConfig.h"
#include "../Game.h"

PhysicsComponent::PhysicsComponent()
{

}

PhysicsComponent::PhysicsComponent(Json::Value definitionJSON, float xMapPos, float yMapPos, float angleAdjust)
{
	//Get reference to the animationComponent JSON config and transformComponent JSON config
	Json::Value physicsComponentJSON = definitionJSON["physicsComponent"];
	Json::Value transformComponentJSON = definitionJSON["transformComponent"];

	m_physicsType = EnumMap::instance().toEnum(physicsComponentJSON["type"].asString());
	m_collisionShape = EnumMap::instance().toEnum(physicsComponentJSON["collisionShape"].asString());
	m_collisionRadius = physicsComponentJSON["collisionRadius"].asFloat();
	m_friction = physicsComponentJSON["friction"].asFloat();
	m_density = physicsComponentJSON["density"].asFloat();
	m_restitution = physicsComponentJSON["restitution"].asFloat();
	m_linearDamping = physicsComponentJSON["linearDamping"].asFloat();
	m_angularDamping = physicsComponentJSON["angularDamping"].asFloat();
	m_collisionCategory = EnumMap::instance().toEnum(physicsComponentJSON["collisionCategory"].asString());
	
	m_objectAnchorPoint.Set(physicsComponentJSON["anchorPoint"]["x"].asFloat(),
		physicsComponentJSON["anchorPoint"]["y"].asFloat());

	//Build the physics body
	m_physicsBody = _buildB2Body(transformComponentJSON);

	//Calculate the spawn position
	//Translate the pixel oriented position into box2d meter-oriented
	b2Vec2* position = new b2Vec2
	(  (xMapPos * Game::instance().worldTileWidth() + (transformComponentJSON["size"]["width"].asFloat() / 2)) / GameConfig::instance().scaleFactor(),
		(yMapPos * Game::instance().worldTileHeight() + (transformComponentJSON["size"]["height"].asFloat() / 2)) / GameConfig::instance().scaleFactor());

	//Calculate the spawn Angle
	float newAngle = util::degreesToRadians(angleAdjust);

	//Initial spawn position
	//FIXME:Need to pass in position info
	m_physicsBody->SetTransform(*position, newAngle);

}

PhysicsComponent::~PhysicsComponent()
{

}
void PhysicsComponent::setTransform(b2Vec2 positionVector, float angle)
{
	m_physicsBody->SetTransform(positionVector, angle);

}

void PhysicsComponent::setPhysicsBodyActive(bool  active)
{
	m_physicsBody->SetActive(active);

}

void PhysicsComponent::setLinearVelocity(b2Vec2 velocityVector)
{
	m_physicsBody->SetLinearVelocity(velocityVector);
}

void PhysicsComponent::update()
{
	//update the UserData - only once - cant do it in the constructor
	if (m_physicsBody->GetUserData() == nullptr)
	{
		m_physicsBody->SetUserData(gameObject.get());
	}

	//Transfer the physicsComponent coordinates to the transformComponent
	b2Vec2 convertedPosition{ 0,0 };
	float convertedAngle = util::radiansToDegrees(m_physicsBody->GetAngle());

	convertedPosition.x = m_physicsBody->GetPosition().x * GameConfig::instance().scaleFactor();
	convertedPosition.y = m_physicsBody->GetPosition().y * GameConfig::instance().scaleFactor();

	gameObject->getComponent<TransformComponent>()->setPosition(convertedPosition, convertedAngle);
}

b2Body* PhysicsComponent::_buildB2Body(Json::Value transformComponentJSON)
{
	b2BodyDef bodyDef;

	bodyDef.type = static_cast<b2BodyType>(m_physicsType);

	//Default the position to zero.
	bodyDef.position.SetZero();
	bodyDef.allowSleep = true;
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
	uint16 mask = this->_setCollisionMask(m_collisionCategory);
	fixtureDef.filter.maskBits = mask;

	// Add the shape to the body.
	body->CreateFixture(&fixtureDef);

	body->SetLinearDamping(m_linearDamping);
	body->SetAngularDamping(m_angularDamping);

	//body->SetUserData(m_parentGameObject.get());
	//this->box2dBodyCount++;
	return body;

}

uint16 PhysicsComponent::_setCollisionMask(uint16 category)
{
	uint16 mask = 0;


	switch (category) {
	case COLLISION_GENERIC:
		mask = COLLISION_WALL | COLLISION_PARTICLE2 | COLLISION_PARTICLE3 | COLLISION_ENEMY_FRAME
			| COLLISION_ENEMY_ARMOR_PIECE | COLLISION_PLAYER;
		break;

	case COLLISION_PLAYER:
		mask = COLLISION_WALL | COLLISION_PARTICLE2 | COLLISION_PARTICLE3 | COLLISION_ENEMY_FRAME
			| COLLISION_ENEMY_ARMOR_PIECE | COLLISION_GENERIC;
		break;
	case COLLISION_WALL:
		mask = COLLISION_PLAYER | COLLISION_PARTICLE1 | COLLISION_PARTICLE2 | COLLISION_PARTICLE3
			| COLLISION_ENEMY_FRAME | COLLISION_PLAYER_BULLET | COLLISION_ENEMY_ARMOR_PIECE | COLLISION_GENERIC;
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


void PhysicsComponent::applyMovement(float velocity, int direction, int strafeDirection)
{


	//Calc direction XY
	//float dx = cos(this->physicsBody->GetAngle()) * velocity * this->direction; // X-component.
	//float dy = sin(this->physicsBody->GetAngle()) * velocity * this->direction; // Y-component.
	float dx = cos(1.5708) * velocity * direction; // X-component.
	float dy = sin(1.5708) * velocity * direction; // Y-component.

	//calc strafe xy and add direction and strafe vectors
	//1.5708 is 90 degrees
	//float sx = cos(this->physicsBody->GetAngle() + 1.5708) * velocity * this->strafe; // X-component.
	//float sy = sin(this->physicsBody->GetAngle() + 1.5708) * velocity * this->strafe; // Y-component.
	float sx = cos((1.5708) + 1.5708) * velocity * strafeDirection; // X-component.
	float sy = sin((1.5708) + 1.5708) * velocity * strafeDirection; // Y-component.

	//Create the vector for forward/backward  direction
	b2Vec2 directionVector = b2Vec2(dx, dy);

	//Create the vector for strafe direction
	b2Vec2 strafeVector = b2Vec2(sx, sy);

	//Initialize new final movement vector
	b2Vec2 vec2;
	vec2.SetZero();

	vec2 = (directionVector + strafeVector);

	//Update Animation state
	/*if (vec2.Length() > 0)
	{
		this->setCurrentAnimationState("RUN");
	}
	else
	{
		this->setCurrentAnimationState("IDLE");
	}*/

	//this->physicsBody->SetTransform(vec3, this->physicsBody->GetAngle());
	m_physicsBody->SetLinearVelocity(vec2);

	//m_physicsBody->ApplyLinearImpulseToCenter(vec2, true);


}

void PhysicsComponent::applyRotation(float angularVelocity)
{
	m_physicsBody->SetAngularVelocity(angularVelocity);
}

void PhysicsComponent::setOffGrid()
{
	b2Vec2 velocityVector = b2Vec2(0, 0);
	b2Vec2 positionVector = b2Vec2(-50, -50);

	m_physicsBody->SetTransform(positionVector, 0);
	m_physicsBody->SetLinearVelocity(velocityVector);
	m_physicsBody->SetActive(false);
}

void PhysicsComponent::attachItem(std::shared_ptr<GameObject>inventoryObject)
{
	//Get physics component of the inventory object
	auto& inventoryObjectPhysicsComponent = inventoryObject->getComponent<PhysicsComponent>();

	b2WeldJointDef weldJointDef;
	weldJointDef.referenceAngle;
	weldJointDef.bodyA = m_physicsBody;
	weldJointDef.bodyB = inventoryObjectPhysicsComponent->m_physicsBody;
	weldJointDef.collideConnected = false;

	b2Vec2 worldObjectAnchorPoint = {
		m_objectAnchorPoint.x,
		m_objectAnchorPoint.y
	};
	weldJointDef.localAnchorA = worldObjectAnchorPoint;

	b2Vec2 weaponsAnchorPoint = {
		inventoryObjectPhysicsComponent->m_objectAnchorPoint.x,
		inventoryObjectPhysicsComponent->m_objectAnchorPoint.y
	};
	weldJointDef.localAnchorB = weaponsAnchorPoint;
	(b2WeldJointDef*)Game::instance().physicsWorld()->CreateJoint(&weldJointDef);

}