#include "PhysicsComponent.h"


#include "../EnumMap.h"
#include "../Game.h"

extern std::unique_ptr<Game> game;

PhysicsComponent::PhysicsComponent(Json::Value definitionJSON, Scene* parentScene, float xMapPos, float yMapPos, float angleAdjust)
{

	m_componentType = ComponentTypes::PHYSICS_COMPONENT;

	//Get reference to the animationComponent JSON config and transformComponent JSON config
	Json::Value physicsComponentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::PHYSICS_COMPONENT);
	Json::Value transformComponentJSON = util::getComponentConfig(definitionJSON, ComponentTypes::TRANSFORM_COMPONENT);

	m_physicsType = game->enumMap()->toEnum(physicsComponentJSON["type"].asString());

	m_objectAnchorPoint.Set(physicsComponentJSON["anchorPoint"]["x"].asFloat(),
		physicsComponentJSON["anchorPoint"]["y"].asFloat());

	//Build the physics body
	m_physicsBody = _buildB2Body(physicsComponentJSON, transformComponentJSON, parentScene->physicsWorld());

	//Calculate the spawn position
	b2Vec2 position{};

	//Get size of the object
	float objectWidth = transformComponentJSON["size"]["width"].asFloat();
	float objectHeight = transformComponentJSON["size"]["height"].asFloat();

	//Angle adjustment if any in radians
	float newAngle = util::degreesToRadians(angleAdjust);

	//The width and height can change when a rectangle shape is rotated
	float objectWidthAfterAngle = abs(sin(newAngle) * objectHeight + cos(newAngle) * objectWidth);
	float objectHeightAfterAngle = abs(sin(newAngle) * objectWidth + cos(newAngle) * objectHeight);

	//Get the pixel position of where we are placing the object. Divide by 2 to get the center
	position.x = (xMapPos * game->worldTileSize().x + (objectWidthAfterAngle / 2));
	position.y = (yMapPos * game->worldTileSize().y + (objectHeightAfterAngle / 2));

	//Scale them to the box2d size
	position = util::toBox2dPoint(position);

	//Initial spawn position
	m_physicsBody->SetTransform(position, newAngle);

}

PhysicsComponent::~PhysicsComponent()
{

	//We need to free the memory associated with our special object that we store in each fixture's userdata
	//NOTE:this should be the only spot where we do not depend on smart pointers
	//std::cout << this->parent()->id() << " PhysicsComponent Destructor called" << std::endl;
	for (auto fixture = m_physicsBody->GetFixtureList(); fixture != 0; fixture = fixture->GetNext())
	{

		ContactDefinition* contactDefinition = reinterpret_cast<ContactDefinition*>(fixture->GetUserData().pointer);
		if (contactDefinition) {
			delete contactDefinition;
		}

	}

	parent()->parentScene()->physicsWorld()->DestroyBody(m_physicsBody);

}

void PhysicsComponent::postInit()
{

	
}

void PhysicsComponent::setParent(GameObject* gameObject)
{
	Component::setParent(gameObject);
	m_physicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(gameObject);
}

void PhysicsComponent::setTransform(b2Vec2 positionVector, float angle)
{
	m_physicsBody->SetTransform(positionVector, angle);
}

void PhysicsComponent::setTransform(b2Vec2 positionVector)
{
	m_physicsBody->SetTransform(positionVector, m_physicsBody->GetAngle());
}

void PhysicsComponent::setPhysicsBodyActive(bool  active)
{
	m_physicsBody->SetEnabled(active);

}

void PhysicsComponent::setLinearVelocity(b2Vec2 velocityVector)
{
	m_physicsBody->SetLinearVelocity(velocityVector);
}

void PhysicsComponent::setLinearDamping(float linearDamping)
{
	m_physicsBody->SetLinearDamping(linearDamping);
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

	//If we were given a position change from our contactListener, since we cannnot change position within
	//contactListener, we set it here. An example would be warping the player to a new location after making contact
	//with a portal object
	b2Vec2 currentPosition = { m_physicsBody->GetPosition().x , m_physicsBody->GetPosition().y };
	if (m_changePositionPosition.has_value()) {
		currentPosition = { m_changePositionPosition->x , m_changePositionPosition->y };
		m_changePositionPosition.reset();
	}
	
	//Set the transform
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

	if (physicsComponentJSON.isMember("linearDamping")) {
		body->SetLinearDamping(physicsComponentJSON["linearDamping"].asFloat());
	}
	if (physicsComponentJSON.isMember("angularDamping")) {
		body->SetAngularDamping(physicsComponentJSON["angularDamping"].asFloat());
	}
	if (physicsComponentJSON.isMember("gravityScale")) {
		body->SetGravityScale(physicsComponentJSON["gravityScale"].asFloat());
	}
	if (physicsComponentJSON.isMember("isBullet")) {
		body->SetBullet(physicsComponentJSON["isBullet"].asBool());
	}

	//Build fixtures
	for (const auto& fixtureJSON : physicsComponentJSON["fixtures"]) {

		b2Shape* shape;
		b2PolygonShape box;
		b2CircleShape circle;
		b2ChainShape chain;
		b2EdgeShape edge;

		auto collisionShape = game->enumMap()->toEnum(fixtureJSON["collisionShape"].asString());
		//default
		shape = &box;

		if (collisionShape == b2Shape::e_circle)
		{
			circle.m_radius = fixtureJSON["collisionRadius"].asFloat();
			shape = &circle;
		}
		else if (collisionShape == b2Shape::e_polygon) {

			float sizeX{};
			float sizeY{};
			//If a size is not specified for the fixture then default to the transform size of the object
			if (fixtureJSON.isMember("size")) {
				sizeX = fixtureJSON["size"]["width"].asFloat() / GameConfig::instance().scaleFactor() / 2;
				sizeY = fixtureJSON["size"]["height"].asFloat() / GameConfig::instance().scaleFactor() / 2;
			}
			else {
				sizeX = transformComponentJSON["size"]["width"].asFloat() / GameConfig::instance().scaleFactor() / 2;
				sizeY = transformComponentJSON["size"]["height"].asFloat() / GameConfig::instance().scaleFactor() / 2;
			}
			
			box.SetAsBox(sizeX, sizeY);
			shape = &box;
		}
		else if (collisionShape == b2Shape::e_chain) {

			PhysicsChainType physicsChainType = (PhysicsChainType)game->enumMap()->toEnum(fixtureJSON["physicsChainType"].asString());

			float sizeX{};
			float sizeY{};
			//If a size is not specified for the fixture then default to the transform size of the object
			if (fixtureJSON.isMember("size")) {
				sizeX = fixtureJSON["size"]["width"].asFloat() / GameConfig::instance().scaleFactor();
				sizeY = fixtureJSON["size"]["height"].asFloat() / GameConfig::instance().scaleFactor();
			}
			else {
				sizeX = transformComponentJSON["size"]["width"].asFloat() / GameConfig::instance().scaleFactor();
				sizeY = transformComponentJSON["size"]["height"].asFloat() / GameConfig::instance().scaleFactor();
			}

			b2Vec2 chainVs[4];
			if (physicsChainType == PhysicsChainType::CW_REFLECT_OUT) {
				chainVs[0].Set(-sizeX/2, -sizeY/2);
				chainVs[1].Set(sizeX/2 , -sizeY/2);
				chainVs[2].Set(sizeX/2, sizeY/2);
				chainVs[3].Set(-sizeX/2, sizeY/2);
			}
			else if (physicsChainType == PhysicsChainType::CCW_REFLECT_IN) {
				chainVs[0].Set(-sizeX / 2, sizeY / 2);
				chainVs[1].Set(sizeX / 2, sizeY / 2);
				chainVs[2].Set(sizeX / 2, -sizeY / 2);
				chainVs[3].Set(-sizeX / 2, -sizeY / 2);
			}

			chain.CreateLoop(chainVs, 4);
			shape = &chain;
		}

		b2FixtureDef fixtureDef;
		fixtureDef.shape = shape;
		fixtureDef.friction = fixtureJSON["friction"].asFloat();
		fixtureDef.density = fixtureJSON["density"].asFloat();
		fixtureDef.restitution = fixtureJSON["restitution"].asFloat();
		fixtureDef.isSensor = fixtureJSON["isSensor"].asFloat();

		ContactDefinition* contactDefinition = new ContactDefinition();
		contactDefinition->contactTag = game->enumMap()->toEnum(fixtureJSON["contactTag"].asString());
		contactDefinition->saveOriginalContactTag = game->enumMap()->toEnum(fixtureJSON["contactTag"].asString());
		fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(contactDefinition);

		body->CreateFixture(&fixtureDef);

	}

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

	//m_physicsBody->ApplyLinearImpulseToCenter(trajectory, true);
	m_physicsBody->ApplyLinearImpulseToCenter(trajectory, true);

}

void PhysicsComponent::applyImpulse(float speed, int direction, int strafeDirection)
{
	b2Vec2 trajectory = { (float)strafeDirection, (float)direction };
	trajectory.Normalize();

	trajectory *= speed;

	//apply point
	b2Vec2 applyPoint{2, -2};

	//m_physicsBody->ApplyLinearImpulseToCenter(trajectory, true);
	m_physicsBody->ApplyForce(trajectory, applyPoint, true);

}

void PhysicsComponent::applyMovement(float velocity, b2Vec2 trajectory)
{

	trajectory.x *= velocity;
	trajectory.y *= velocity;

	//m_physicsBody->ApplyForceToCenter(trajectory, true);
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

void PhysicsComponent::deleteAllJoints()
{
	std::vector< b2JointEdge*> joints{};

	//We need to store the joints in an array first , then delete them
	//deleting them as you go will mess up the ->next
	for (b2JointEdge* joint = physicsBody()->GetJointList(); joint; joint = joint->next)
	{
		joints.push_back(joint);
	}

	for (auto joint : joints) {

		parent()->parentScene()->physicsWorld()->DestroyJoint(joint->joint);

	}


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

		////test
		//revoluteJointDef->enableMotor = true;
		//revoluteJointDef->maxMotorTorque = 100;
		
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


