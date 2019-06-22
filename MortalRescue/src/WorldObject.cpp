#include "WorldObject.h"

#include "game.h"



WorldObject::WorldObject()
{

}

WorldObject::WorldObject(string gameObjectId, int xMapPos, int yMapPos, int angleAdjust) :
	GameObject(gameObjectId, xMapPos, yMapPos, angleAdjust)
{
	//Build the box2d object
	if (this->definition->isPhysicsObject == true)
	{
		this->physicsBody = buildB2Body(this->definition);
		//Add a reference to the gameObject itself to the physics object for collision helping logic later
		this->physicsBody->SetUserData(this);

	}

	//build the animation objects
	/*
	for (auto & gameObjectDefinitionAnimation : gameObjectDefinition->animations) {

		gameObject->animations[gameObjectDefinitionAnimation.second.id] = gameObjectDefinitionAnimation.second;

	}
	*/

}

WorldObject::~WorldObject()
{
}


void WorldObject::update()
{
	GameObject::update();

}

void WorldObject::render()
{

	Game::textureManager.render(this);
}

b2Body * WorldObject::buildB2Body(GameObjectDefinition* gameObjectDefinition)
{
	b2BodyDef bodyDef;

	if (gameObjectDefinition->physicsType.compare("B2_STATIC") == 0)
	{
		bodyDef.type = b2_staticBody;
	}
	else if (gameObjectDefinition->physicsType.compare("B2_KINEMATIC") == 0)
	{
		bodyDef.type = b2_kinematicBody;
	}
	else if (gameObjectDefinition->physicsType.compare("B2_DYNAMIC") == 0)
	{
		bodyDef.type = b2_dynamicBody;
	}

	bodyDef.position.Set(gameObjectDefinition->initPosX, gameObjectDefinition->initPosY);
	b2Body* body = Game::physicsWorld->CreateBody(&bodyDef);

	b2Shape* shape;
	b2PolygonShape box;
	b2CircleShape circle;

	//Temp - Circle shape for player
	if (gameObjectDefinition->id == "GINA_64") {

		//circle.m_p.Set(2.0f, 3.0f);
		circle.m_radius = .96f;
		shape = &circle;

	}
	else
	{
		//Box Shape
		float32 xSize = gameObjectDefinition->xSize / 2; //SetAsBox takes half-widths
		float32 YSize = gameObjectDefinition->ySize / 2;
		box.SetAsBox(xSize, YSize);
		shape = &box;
		//std::cout << "BuildBox size for " << gameObjectDefinition->id << " was " << xSize << "\n";
	}

	// Define the body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = shape;

	// Misc properties
	fixtureDef.density = gameObjectDefinition->density;
	fixtureDef.friction = gameObjectDefinition->friction;
	fixtureDef.restitution = 0.0;

	/*
	if (gameObjectDefinition->id == "BULLET2")
	{
		fixtureDef.filter.groupIndex = -1;
	}
	*/

	// Add the shape to the body.
	body->CreateFixture(&fixtureDef);

	body->SetLinearDamping(gameObjectDefinition->linearDamping);
	body->SetAngularDamping(gameObjectDefinition->angularDamping);

	//this->box2dBodyCount++;
	return body;

}
