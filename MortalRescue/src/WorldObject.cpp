#include "WorldObject.h"
#include "GameObjectDefinition.h"

#include "game.h"



WorldObject::WorldObject()
{

}

WorldObject::WorldObject(string gameObjectId, int xMapPos, int yMapPos, int angleAdjust) :
	GameObject(gameObjectId, xMapPos, yMapPos, angleAdjust)
{
	//pointer to the definition
	//GameObjectDefinition* definition = game->gameObjectManager.gameObjectDefinitions[gameObjectId];

	//Gameobject must be passed in it's starting position
	//Multiply the size times the x,y position in the map grid that represents the world
	//When buildB2Body executes, it will build the box2d object centered on the x,y position we give,
	// We need it centered on the grid location
	//so add half of the object size so that the object will be placed with its top left corner in the grid location
	//we specify
	this->xPos = (xMapPos * (game->worldGridSize.w / game->config.scaleFactor)) + (definition->xSize / 2);
	this->yPos = (yMapPos * (game->worldGridSize.h / game->config.scaleFactor)) + (definition->ySize / 2);

	//speed
	this->speed = definition->speed;

	//Build box2d related stuff
	this->physicsBody = buildB2Body(definition);
	b2Vec2 positionVector = b2Vec2( this->xPos, this->yPos);
	this->physicsBody->SetTransform(positionVector, angleAdjust * DEGTORAD);
	//Add a reference to the gameObject itself to the physics object for collision helping logic later
	this->physicsBody->SetUserData(this);

}

WorldObject::~WorldObject()
{
}


void WorldObject::update()
{
	GameObject::update();

}

SDL_Rect* WorldObject::getRenderDestRect(SDL_Rect* destRect)
{
	destRect->w = (this->xSize * game->config.scaleFactor);
	destRect->h = (this->ySize * game->config.scaleFactor);
	destRect->x = round((this->physicsBody->GetPosition().x *  game->config.scaleFactor) - (destRect->w / 2));
	destRect->y = round((this->physicsBody->GetPosition().y *  game->config.scaleFactor) - (destRect->h / 2));

	//Adjust position based on current camera position - offset
	destRect->x -= game->camera.frame.x;
	destRect->y -= game->camera.frame.y;

	return destRect;
}


void WorldObject::render()
{
	SDL_Rect *textureSourceRect = NULL, destRect;
	SDL_Texture* texture = NULL;

	//Get render destination rectangle
	this->getRenderDestRect(&destRect);

	//Get texture

	texture = this->getRenderTexture(texture);

	//Get render texture src rectangle
	textureSourceRect = this->getRenderTextureRect(textureSourceRect);


	//Get the angle of the object and convert it from Radians to Degrees for SDL
	float angle = this->physicsBody->GetAngle();
	angle = angle * 180 / M_PI;

	game->textureManager.renderTexture(texture, textureSourceRect, &destRect, angle);

}

b2Body * WorldObject::buildB2Body(GameObjectDefinition* definition)
{
	b2BodyDef bodyDef;

	if (definition->physicsType.compare("B2_STATIC") == 0)
	{
		bodyDef.type = b2_staticBody;
	}
	else if (definition->physicsType.compare("B2_KINEMATIC") == 0)
	{
		bodyDef.type = b2_kinematicBody;
	}
	else if (definition->physicsType.compare("B2_DYNAMIC") == 0)
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
	if(definition->collisionShape.compare("B2_CIRCLE") == 0)
	{
		circle.m_radius = definition->collisionRadius;
		shape = &circle;
	}
	else
	{
		//Box Shape
		float32 xSize = definition->xSize / 2; //SetAsBox takes half-widths
		float32 YSize = definition->ySize / 2;
		box.SetAsBox(xSize, YSize);
		shape = &box;
	}

	// Define the body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = shape;

	// Misc properties
	fixtureDef.density = definition->density;
	fixtureDef.friction = definition->friction;
	fixtureDef.restitution = definition->restitution;
	//collision group
	fixtureDef.filter.groupIndex = definition->collisionGroup;



	// Add the shape to the body.
	body->CreateFixture(&fixtureDef);

	body->SetLinearDamping(definition->linearDamping);
	body->SetAngularDamping(definition->angularDamping);

	//this->box2dBodyCount++;
	return body;

}
