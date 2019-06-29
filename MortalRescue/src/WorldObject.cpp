#include "WorldObject.h"

#include "game.h"



WorldObject::WorldObject()
{

}

WorldObject::WorldObject(string gameObjectId, int xMapPos, int yMapPos, int angleAdjust) :
	GameObject(gameObjectId, xMapPos, yMapPos, angleAdjust)
{

	//Gameobject must be passed in it's starting position
	//Multiply the size times the x,y position in the map grid that represents the world
	//When buildB2Body executes, it will build the box2d object centered on the x,y position we give,
	// We need it centered on the grid location
	//so add half of the object size so that the object will be placed with its top left corner in the grid location
	//we specify
	this->xPos = (xMapPos * (game->worldGridSize.w / game->config.scaleFactor)) + (this->definition->xSize / 2);
	this->yPos = (yMapPos * (game->worldGridSize.h / game->config.scaleFactor)) + (this->definition->ySize / 2);

	//Build box2d related stuff
	this->physicsBody = buildB2Body(this->definition);
	b2Vec2 positionVector = b2Vec2( this->xPos, this->yPos);
	this->physicsBody->SetTransform(positionVector, angleAdjust * DEGTORAD);
	//Add a reference to the gameObject itself to the physics object for collision helping logic later
	this->physicsBody->SetUserData(this);

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

	game->textureManager.render(this);
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

	//Default the position to zero.
	bodyDef.position.SetZero();
	b2Body* body = game->physicsWorld->CreateBody(&bodyDef);

	b2Shape* shape;
	b2PolygonShape box;
	b2CircleShape circle;

	//Collision shape - will default to a rectangle
	if(gameObjectDefinition->collisionShape.compare("B2_CIRCLE") == 0) 
	{
		circle.m_radius = gameObjectDefinition->collisionRadius;
		shape = &circle;
	}
	else
	{
		//Box Shape
		float32 xSize = gameObjectDefinition->xSize / 2; //SetAsBox takes half-widths
		float32 YSize = gameObjectDefinition->ySize / 2;
		box.SetAsBox(xSize, YSize);
		shape = &box;
	}

	// Define the body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = shape;

	// Misc properties
	fixtureDef.density = gameObjectDefinition->density;
	fixtureDef.friction = gameObjectDefinition->friction;
	fixtureDef.restitution = 0.0;

	// Add the shape to the body.
	body->CreateFixture(&fixtureDef);

	body->SetLinearDamping(gameObjectDefinition->linearDamping);
	body->SetAngularDamping(gameObjectDefinition->angularDamping);

	//this->box2dBodyCount++;
	return body;

}
