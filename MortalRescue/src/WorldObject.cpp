#include "WorldObject.h"

#include "GameObjectDefinition.h"
#include "TextureManager.h"
#include "WeaponObject.h"

#include "GameConfig.h"
#include "Game.h"



WorldObject::WorldObject()
{

}

WorldObject::WorldObject(string gameObjectId, float xMapPos, float yMapPos, float angleAdjust) :
	GameObject(gameObjectId, xMapPos, yMapPos, angleAdjust)
{
	//Size
	//FIXME:Add a override function to setSize to worldPbject to automatically multiply the scalefactor
	this->setSize(this->definition()->xSize * GameConfig::instance().scaleFactor(), 
		this->definition()->ySize * GameConfig::instance().scaleFactor());

	//speed
	m_speed = this->definition()->speed;

	//Build box2d related stuff
	m_physicsBody = buildB2Body(this->definition());

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
	//m_physicsBody->SetUserData(this);
	this->setBox2DUserData(this);

}

WorldObject::~WorldObject()
{
}


void WorldObject::setPosition(b2Vec2 position, float angle)
{
	b2Vec2 newlocation;
	//newlocation.x = (position.x / GameConfig::instance().scaleFactor);
	//newlocation.y = (position.y / GameConfig::instance().scaleFactor);

	newlocation.x = (position.x / GameConfig::instance().scaleFactor()) + (this->definition()->xSize / 2);
	newlocation.y = (position.y / GameConfig::instance().scaleFactor()) + (this->definition()->ySize / 2);

	m_physicsBody->SetTransform(newlocation, angle );
	//m_physicsBody->SetLinearVelocity(b2Vec2(0, 0));
	//m_physicsBody->SetAngularVelocity(0);

}

void WorldObject::update()
{
	//transfer the angle from the physics body to the main game 
	//object so that certain gamObject logic will work for all
	this->setAngle(m_physicsBody->GetAngle());
	//this->xPos = m_physicsBody->GetTransform().p.x;
	//this->yPos = m_physicsBody->GetTransform().p.y;



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
	positionRect.x = round((m_physicsBody->GetPosition().x * GameConfig::instance().scaleFactor()) - (this->size().x / 2));
	positionRect.y = round((m_physicsBody->GetPosition().y * GameConfig::instance().scaleFactor()) - (this->size().y / 2));
	return positionRect;

}

SDL_Rect WorldObject::getRenderDestRect()
{
	SDL_Rect destRect;
	
	destRect = this->getPositionRect();
	
	destRect.w += this->definition()->xRenderAdjustment;
	destRect.h += this->definition()->yRenderAdjustment;

	//Adjust position based on current camera position - offset
	destRect.x -= Camera::instance().frame().x;
	destRect.y -= Camera::instance().frame().y;

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
	float angle = m_physicsBody->GetAngle();
	angle = angle * (float)180 / M_PI;

	
	if (this->definition()->id.compare("GINA_64") == 0)
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
	
	TextureManager::instance().render(texture, this->color(), textureSourceRect, &destRect, angle);

	//Loop through any possible child objects, in all 9 positions, and render them too
	if (this->definition()->hasChildObjects == true)
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
			mask = WALL | PARTICLE2 | PARTICLE3 | ENEMY_FRAME | ENEMY_ARMOR_PIECE;
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

	m_physicsBody->SetActive(active);

}

bool WorldObject::testStrength(int bulletStrength)
{

	if (bulletStrength >= this->strength()) {
		return true;
	}
	else
	{
		return false;
	}

}

void WorldObject::setBox2DUserData(WorldObject* worldObject)
{

	m_physicsBody->SetUserData(worldObject);

}

void WorldObject::addWeapon(string weaponObjectId)
{

	WeaponObject *weaponObject = 
		GameObjectManager::instance().buildGameObject <WeaponObject>
		(weaponObjectId, 4, 4, 0);

	b2WeldJointDef weldJointDef;
	weldJointDef.referenceAngle;
	weldJointDef.bodyA = this->physicsBody();
	weldJointDef.bodyB = weaponObject->physicsBody();
	weldJointDef.collideConnected = false;
	
	b2Vec2 worldObjectAnchorPoint = { 
		this->definition()->weaponAnchorPoint.x, 
		this->definition()->weaponAnchorPoint.y 
	};
	weldJointDef.localAnchorA = worldObjectAnchorPoint;

	b2Vec2 weaponsAnchorPoint =	{ 
		weaponObject->definition()->weaponDetails.anchorPoint.x, 
		weaponObject->definition()->weaponDetails.anchorPoint.y 
	};
	weldJointDef.localAnchorB = weaponsAnchorPoint;
	(b2WeldJointDef*)game->physicsWorld->CreateJoint(&weldJointDef);

	this->m_weapon = weaponObject;

	game->addGameObject(weaponObject, GameOjectLayer::MAIN);

}