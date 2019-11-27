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

	//Size
	this->xSize = definition->xSize * game->config.scaleFactor;
	this->ySize = definition->ySize * game->config.scaleFactor;

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


void WorldObject::setPosition(b2Vec2 position, float angle)
{
	b2Vec2 newlocation;
	newlocation.x = position.x / game->config.scaleFactor;
	newlocation.y = position.y / game->config.scaleFactor;

	this->physicsBody->SetTransform(newlocation, angle);
}

void WorldObject::update()
{
	//transfer the angle from the physics body to the main game 
	//object so that certain gamObject logic will work for all
	this->angle = this->physicsBody->GetAngle();


	GameObject::update();


}

SDL_Rect WorldObject::getRenderDestRect()
{
	SDL_Rect destRect;
	
	destRect.w = this->xSize;
	destRect.h = this->ySize;

	//World objects position from box2d is the center of the object
	//So, we need to adjust the rectangle top left corner to be
	//the render point for SDL
	destRect.x = round((this->physicsBody->GetPosition().x *  game->config.scaleFactor) - (this->xSize / 2));
	destRect.y = round((this->physicsBody->GetPosition().y *  game->config.scaleFactor) - (this->ySize / 2));

	//Adjust position based on current camera position - offset
	if (this->isChildObject == false)
	{
		destRect.x -= game->camera.frame.x;
		destRect.y -= game->camera.frame.y;
	}

	return destRect;
}


void WorldObject::render()
{
	SDL_Rect *textureSourceRect = NULL, destRect;
	SDL_Texture* texture = NULL;

	//Get render destination rectangle
	destRect = this->getRenderDestRect();

	//Get texture
	texture = this->getRenderTexture(texture);

	//Get render texture src rectangle
	textureSourceRect = this->getRenderTextureRect(textureSourceRect);

	//Get the angle of the object and convert it from Radians to Degrees for SDL
	float angle = this->physicsBody->GetAngle();
	angle = angle * 180 / M_PI;

	game->textureManager.render(texture, textureSourceRect, &destRect, angle);

	//Loop through any possible child objects, in all 9 positions, and render them too
	if (this->definition->hasChildObjects == true)
	{
		GameObject::renderChildObjects();
	}


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

	//collision category
	fixtureDef.filter.categoryBits = definition->collisionCategory;
	uint16 mask = this->setCollisionMask(definition->collisionCategory);
	fixtureDef.filter.maskBits = mask;

	// Add the shape to the body.
	body->CreateFixture(&fixtureDef);

	body->SetLinearDamping(definition->linearDamping);
	body->SetAngularDamping(definition->angularDamping);

	//this->box2dBodyCount++;
	return body;

}

uint16 WorldObject::setCollisionMask(uint16 category)
{
	uint16 mask=0;

	switch(category) {
	case PLAYER:
		mask = WALL | PARTICLE1 | PARTICLE2 | PARTICLE3 | ENEMY_FRAME;
		break;
	case WALL:
		mask = PLAYER | PARTICLE1 | PARTICLE2 | PARTICLE3 | ENEMY_FRAME | PLAYER_BULLET;
		break;
	case PLAYER_BULLET:
		mask = ENEMY_ARMOR | WALL;
		break;
	case PARTICLE1:
		mask = WALL | PLAYER | ENEMY_ARMOR;
		break;
	case PARTICLE2:
		mask = WALL | PLAYER | ENEMY_ARMOR;
		break;
	case PARTICLE3:
		mask = WALL | PLAYER | ENEMY_ARMOR;
		break;
	case ENEMY_FRAME:
		mask = WALL | PLAYER ;
		break;
	case ENEMY_ARMOR:
		mask = PLAYER_BULLET | PARTICLE1 | PARTICLE2 | PARTICLE3;
		break;


	}
		
	return mask;

}
