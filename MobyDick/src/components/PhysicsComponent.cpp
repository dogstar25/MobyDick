#include "PhysicsComponent.h"


#include "../EnumMaps.h"
#include "../Game.h"

extern std::unique_ptr<Game> game;

PhysicsComponent::PhysicsComponent(Json::Value definitionJSON, Scene* parentScene, float xMapPos, float yMapPos, float angleAdjust)
{
	//Get reference to the animationComponent JSON config and transformComponent JSON config
	Json::Value physicsComponentJSON = definitionJSON["physicsComponent"];
	Json::Value transformComponentJSON = definitionJSON["transformComponent"];

	m_gameObjectId = definitionJSON["id"].asString();;

	m_physicsType = EnumMap::instance().toEnum(physicsComponentJSON["type"].asString());
	m_collisionShape = EnumMap::instance().toEnum(physicsComponentJSON["collisionShape"].asString());
	m_collisionRadius = physicsComponentJSON["collisionRadius"].asFloat();
	m_friction = physicsComponentJSON["friction"].asFloat();
	m_density = physicsComponentJSON["density"].asFloat();
	m_restitution = physicsComponentJSON["restitution"].asFloat();
	m_linearDamping = physicsComponentJSON["linearDamping"].asFloat();
	m_angularDamping = physicsComponentJSON["angularDamping"].asFloat();
	m_gravityScale = physicsComponentJSON["gravityScale"].asFloat();

	m_objectAnchorPoint.Set(physicsComponentJSON["anchorPoint"]["x"].asFloat(),
		physicsComponentJSON["anchorPoint"]["y"].asFloat());

	//Build the physics body
	m_physicsBody = _buildB2Body(physicsComponentJSON, transformComponentJSON, parentScene->physicsWorld());

	//Calculate the spawn position
	//Translate the pixel oriented position into box2d meter-oriented
	b2Vec2* position = new b2Vec2
	(  (xMapPos * game->worldTileWidth() + (transformComponentJSON["size"]["width"].asFloat() / 2)) / GameConfig::instance().scaleFactor(),
		(yMapPos * game->worldTileHeight() + (transformComponentJSON["size"]["height"].asFloat() / 2)) / GameConfig::instance().scaleFactor());

	//Calculate the spawn Angle
	float newAngle = util::degreesToRadians(angleAdjust);

	//Initial spawn position
	m_physicsBody->SetTransform(*position, newAngle);

}

PhysicsComponent::~PhysicsComponent()
{

}

void PhysicsComponent::postInit(GameObject* parentGameObject)
{
	m_physicsBody->SetUserData(parentGameObject);
}

void PhysicsComponent::setTransform(b2Vec2 positionVector, float angle)
{
	m_physicsBody->SetTransform(positionVector, angle);
}

void PhysicsComponent::setPhysicsBodyActive(bool  active)
{
	m_physicsBody->SetEnabled(active);

}

void PhysicsComponent::setLinearVelocity(b2Vec2 velocityVector)
{
	m_physicsBody->SetLinearVelocity(velocityVector);
}

void PhysicsComponent::setAngle(float angle)
{
	auto normalizedAngle = util::normalizeRadians(angle);

	b2Vec2 currentPosition = { m_physicsBody->GetPosition().x , m_physicsBody->GetPosition().y };
	m_physicsBody->SetTransform(currentPosition, normalizedAngle);

}


void PhysicsComponent::update()
{
	//We want to make sure that the angle stays in the range of 0 to 360 for various concerns throughtout the game
	//Unfortunately, box2d's only function to set an angle value directly is the setTransform which also takes
	// X and Y position, so we have to send setTransform the current X,Y position as well as the updated angle
	// value 
	auto normalizedAngle = util::normalizeRadians(m_physicsBody->GetAngle());
	b2Vec2 currentPosition = { m_physicsBody->GetPosition().x , m_physicsBody->GetPosition().y };
	m_physicsBody->SetTransform(currentPosition, normalizedAngle);

	//Transfer the physicsComponent coordinates to the transformComponent
	//converting the angle to degrees
	b2Vec2 convertedPosition{ 0,0 };
	float convertedAngle = util::radiansToDegrees(m_physicsBody->GetAngle());

	convertedPosition.x = m_physicsBody->GetPosition().x * GameConfig::instance().scaleFactor();
	convertedPosition.y = m_physicsBody->GetPosition().y * GameConfig::instance().scaleFactor();

	parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT)->setPosition(convertedPosition, convertedAngle);
}

b2Body* PhysicsComponent::_buildB2Body(Json::Value physicsComponentJSON, Json::Value transformComponentJSON, b2World* physicsWorld)
{
	b2BodyDef bodyDef;

	bodyDef.type = static_cast<b2BodyType>(m_physicsType);

	//Default the position to zero.
	bodyDef.position.SetZero();
	bodyDef.allowSleep = true;
	b2Body* body = physicsWorld->CreateBody(&bodyDef);

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
			transformComponentJSON["size"]["height"].asFloat() / GameConfig::instance().scaleFactor() / 2/*,
			b2Vec2(m_objectAnchorPoint.x, m_objectAnchorPoint.y), 0*/	);
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
	//fixtureDef.filter.categoryBits = m_collisionCategory;
	//uint16 mask = CollisionManager::instance().getCollisionMask(m_collisionCategory);
	//fixtureDef.filter.maskBits = mask;

	// Add the shape to the body.
	body->CreateFixture(&fixtureDef);

	body->SetLinearDamping(m_linearDamping);
	body->SetAngularDamping(m_angularDamping);
	body->SetGravityScale(m_gravityScale);

	//body->SetUserData(m_parentGameObject.get());
	//this->box2dBodyCount++;
	return body;

}

uint16 PhysicsComponent::_setCollisionMask(Json::Value physicsComponentJSON)
{
	uint16 mask = 0;

	return mask;

}


void PhysicsComponent::applyImpulse(float force, b2Vec2 trajectory)
{

	trajectory.x *= force;
	trajectory.y *= force;

	m_physicsBody->ApplyLinearImpulseToCenter(trajectory, true);
	//m_physicsBody->ApplyForceToCenter(trajectory, true);

}

void PhysicsComponent::applyMovement(float velocity, b2Vec2 trajectory)
{

	trajectory.x *= velocity;
	trajectory.y *= velocity;

	m_physicsBody->SetLinearVelocity(trajectory);

}

void PhysicsComponent::applyAngleImpulse(float force)
{

	m_physicsBody->ApplyAngularImpulse(force,false);
	//m_physicsBody->ApplyForceToCenter(trajectory, true);

}


//void PhysicsComponent::applyMovement(float speed, int direction, int strafeDirection)
//{
//
//
//	//Calc direction XY
//	//float dx = cos(this->physicsBody->GetAngle()) * velocity * this->direction; // X-component.
//	//float dy = sin(this->physicsBody->GetAngle()) * velocity * this->direction; // Y-component.
//	float dx = (float)cos(1.5708) * speed * direction; // X-component.
//	float dy = (float)sin(1.5708) * speed * direction; // Y-component.
//
//	//calc strafe xy and add direction and strafe vectors
//	//1.5708 is 90 degrees
//	//float sx = cos(this->physicsBody->GetAngle() + 1.5708) * velocity * this->strafe; // X-component.
//	//float sy = sin(this->physicsBody->GetAngle() + 1.5708) * velocity * this->strafe; // Y-component.
//	float sx = (float)cos((1.5708) + 1.5708) * speed * strafeDirection; // X-component.
//	float sy = (float)sin((1.5708) + 1.5708) * speed * strafeDirection; // Y-component.
//
//	//Create the vector for forward/backward  direction
//	b2Vec2 directionVector = b2Vec2(dx, dy);
//
//	//Create the vector for strafe direction
//	b2Vec2 strafeVector = b2Vec2(sx, sy);
//
//	//Initialize new final movement vector
//	b2Vec2 vec2;
//	vec2.SetZero();
//
//	vec2 = (directionVector + strafeVector);
//
//	//this->physicsBody->SetTransform(vec3, this->physicsBody->GetAngle());
//	std::cout << "applyMovement " << vec2.x << " " << vec2.y << "\n";
//	m_physicsBody->SetLinearVelocity(vec2);
//
//	//m_physicsBody->ApplyLinearImpulseToCenter(vec2, true);
//
//
//}

void PhysicsComponent::applyMovement(float speed, int direction, int strafeDirection)
{

	b2Vec2 trajectory = { (float)strafeDirection, (float)direction  };
	trajectory.Normalize();

	trajectory *= speed;

	m_physicsBody->SetLinearVelocity(trajectory);


}
void PhysicsComponent::applyRotation(float angularVelocity)
{
	m_physicsBody->SetAngularVelocity(angularVelocity);
}

void PhysicsComponent::applyTorque(float angularVelocity)
{
	m_physicsBody->ApplyTorque(angularVelocity, true);
}

void PhysicsComponent::setOffGrid()
{
	b2Vec2 velocityVector = b2Vec2(0, 0);
	b2Vec2 positionVector = b2Vec2(-50, -50);

	m_physicsBody->SetTransform(positionVector, 0);
	m_physicsBody->SetLinearVelocity(velocityVector);
	m_physicsBody->SetEnabled(false);
}

void PhysicsComponent::attachItem(GameObject* attachObject, b2JointType jointType, std::optional<b2Vec2> attachLocation)
{
	b2JointDef* jointDef=nullptr;
	b2WeldJointDef* weldJointDef;
	b2RevoluteJointDef* revoluteJointDef;

	//Get physics component of the attachment object
	const auto& attachObjectPhysicsComponent = attachObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);

	//If an attach point was passed in then use it , otherwise use the anchor point defined for the object
	b2Vec2 anchorPoint = { 0,0 };
	if (attachLocation) {
		anchorPoint = attachLocation.value();
	}
	else {
		anchorPoint = {
			m_objectAnchorPoint.x,
			m_objectAnchorPoint.y
		};

	}

	//Get attachment anchor point
	b2Vec2 attachObjectAnchorPoint = {
	attachObjectPhysicsComponent->m_objectAnchorPoint.x,
	attachObjectPhysicsComponent->m_objectAnchorPoint.y
	};


	//test
	//attachObjectAnchorPoint = attachObjectPhysicsComponent.get()->physicsBody()->GetLocalCenter();



	//Build specific joint
	if (jointType == b2JointType::e_weldJoint) {
		weldJointDef = new b2WeldJointDef();
		weldJointDef->bodyA = m_physicsBody;
		weldJointDef->bodyB = attachObjectPhysicsComponent->m_physicsBody;
		weldJointDef->collideConnected = false;
		weldJointDef->localAnchorA = anchorPoint;
		weldJointDef->localAnchorB = attachObjectAnchorPoint;
		jointDef = weldJointDef;
	}
	else if (jointType == b2JointType::e_revoluteJoint) {
		revoluteJointDef = new b2RevoluteJointDef();
		revoluteJointDef->bodyA = m_physicsBody;
		revoluteJointDef->bodyB = attachObjectPhysicsComponent->m_physicsBody;
		revoluteJointDef->collideConnected = false;
		revoluteJointDef->localAnchorA = anchorPoint;
		revoluteJointDef->localAnchorB = attachObjectAnchorPoint;
		
		//test
		//revoluteJointDef->enableMotor = true;
		//revoluteJointDef->motorSpeed = 30;
		//revoluteJointDef->maxMotorTorque = 30000;

		jointDef = revoluteJointDef;
	}

	parent()->parentScene()->physicsWorld()->CreateJoint(jointDef);

}

void PhysicsComponent::setFixedRotation(bool fixedRotation)
{
	m_physicsBody->SetFixedRotation(fixedRotation);
}

void PhysicsComponent::setBullet(bool isBullet)
{
	m_physicsBody->SetBullet(isBullet);
}


