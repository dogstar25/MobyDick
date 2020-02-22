#include "WorldObject.h"
#include "GameObjectDefinition.h"

#include "Game.h"



WorldObject::WorldObject()
{

}

WorldObject::WorldObject(string gameObjectId, float xMapPos, float yMapPos, float angleAdjust) :
	GameObject(gameObjectId, xMapPos, yMapPos, angleAdjust)
{
	//Size
	//FIXME:Add a override function to setSize to worldPbject to automatically multiply the scalefactor
	this->setSize(definition->xSize * game->config.scaleFactor, 
		definition->ySize * game->config.scaleFactor);

	//speed
	this->speed = definition->speed;

	//Build box2d related stuff
	this->physicsBody = buildB2Body(definition);

	//Gameobject must be passed in it's starting position
	//Multiply the size times the x,y position in the map grid that represents the world
	//When buildB2Body executes, it will build the box2d object centered on the x,y position we give,
	// We need it centered on the grid location
	//so add half of the object size so that the object will be placed with its top left corner in the grid location
	//we specify
	b2Vec2 position(xMapPos * game->worldGridSize.w, yMapPos * game->worldGridSize.h);
	float angle = util::degreesToRadians(angleAdjust);
	this->setPosition(position, angle);

	//Add a reference to the gameObject itself to the physics object for collision helping logic later
	//this->physicsBody->SetUserData(this);
	this->setBox2DUserData(this);

}

WorldObject::~WorldObject()
{
}


void WorldObject::setPosition(b2Vec2 position, float angle)
{
	b2Vec2 newlocation;
	//newlocation.x = (position.x / game->config.scaleFactor);
	//newlocation.y = (position.y / game->config.scaleFactor);

	newlocation.x = (position.x / game->config.scaleFactor) + (definition->xSize / 2);
	newlocation.y = (position.y / game->config.scaleFactor) + (definition->ySize / 2);

	this->physicsBody->SetTransform(newlocation, angle );
	//this->physicsBody->SetLinearVelocity(b2Vec2(0, 0));
	//this->physicsBody->SetAngularVelocity(0);

}

void WorldObject::update()
{
	//transfer the angle from the physics body to the main game 
	//object so that certain gamObject logic will work for all
	this->setAngle(this->physicsBody->GetAngle());
	//this->xPos = this->physicsBody->GetTransform().p.x;
	//this->yPos = this->physicsBody->GetTransform().p.y;



	GameObject::update();


}

SDL_Rect WorldObject::getPositionRect()
{
	SDL_Rect positionRect;

	positionRect.w = this->size().x;
	positionRect.h = this->size().y;

	//World objects position from box2d is the center of the object
	//So, we need to adjust the rectangle top left corner to be
	//the render point for SDL
	positionRect.x = round((this->physicsBody->GetPosition().x * game->config.scaleFactor) - (this->size().x / 2));
	positionRect.y = round((this->physicsBody->GetPosition().y * game->config.scaleFactor) - (this->size().y / 2));
	return positionRect;

}

SDL_Rect WorldObject::getRenderDestRect()
{
	SDL_Rect destRect;
	
	destRect = this->getPositionRect();
	
	destRect.w += this->definition->xRenderAdjustment;
	destRect.h += this->definition->yRenderAdjustment;

	//Adjust position based on current camera position - offset
	destRect.x -= game->camera.frame.x;
	destRect.y -= game->camera.frame.y;

	return destRect;
}


void WorldObject::render()
{
	SDL_Rect* textureSourceRect=NULL, destRect;
	SDL_Texture* texture = NULL;

	//Get render destination rectangle
	destRect = this->getRenderDestRect();

	//Get texture
	texture = this->getRenderTexture();

	//Get render texture src rectangle
	textureSourceRect = this->getRenderTextureRect();

	//Get the angle of the object and convert it from Radians to Degrees for SDL
	float angle = this->physicsBody->GetAngle();
	angle = angle * (float)180 / M_PI;

	
	if (this->definition->id.compare("GINA_64") == 0)
	{
		if (textureSourceRect == NULL) {
			game->debugPanel->addItem("textureSourceRec", "NULL");
		}
		else
		{
			game->debugPanel->addItem("textureSourceRecX", to_string(textureSourceRect->x));
			game->debugPanel->addItem("textureSourceRecY", to_string(textureSourceRect->y));
		}
		

	}
	
	game->textureManager.render(texture, this->color(), textureSourceRect, &destRect, angle);

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
			mask = WALL | PARTICLE1 | PARTICLE2 | PARTICLE3 | ENEMY_FRAME | ENEMY_ARMOR_PIECE;
			break;
		case WALL:
			mask = PLAYER | PARTICLE1 | PARTICLE2 | PARTICLE3 | ENEMY_FRAME | PLAYER_BULLET | ENEMY_ARMOR_PIECE;
			break;
		case PLAYER_BULLET:
			mask = WALL | ENEMY_ARMOR;
			break;
		case PARTICLE1:
			mask = WALL | PLAYER;
			break;
		case PARTICLE2:
			mask = WALL | PLAYER;
			break;
		case PARTICLE3:
			mask = WALL | PLAYER;
			break;
		case ENEMY_FRAME:
			mask = WALL | PLAYER | ENEMY_ARMOR_PIECE;
			break;
		case ENEMY_ARMOR:
			mask = PLAYER_BULLET;
			break;
		case ENEMY_ARMOR_PIECE:
			mask = PLAYER | WALL | ENEMY_FRAME | ENEMY_ARMOR_PIECE;
			break;


	}
		
	return mask;

}

void WorldObject::setActive(bool active)
{

	this->physicsBody->SetActive(active);

}

bool WorldObject::testStrength(int bulletStrength)
{

	if (bulletStrength >= this->strength) {
		return true;
	}
	else
	{
		return false;
	}

}

void WorldObject::setBox2DUserData(WorldObject* worldObject)
{

	this->physicsBody->SetUserData(worldObject);

}