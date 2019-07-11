#include "Weapon.h"
#include "GameObject.h"
#include "WorldObject.h"
#include "PlayerObject.h"
#include "game.h"
#include <string>

extern Game* game;

Weapon::Weapon()
{
}


Weapon::~Weapon()
{
}

void Weapon::init(string bulletGameObjectId, PlayerObject* weaponWieldingObject, float xOffsetPct, float yOffsetPct)
{
	
	this->bulletGameObjectId = bulletGameObjectId;
	this->weaponWieldingObject = weaponWieldingObject;

	//Calulate the actual offest amount using the percentages that were sent in
	this->xOffset = xOffsetPct * this->weaponWieldingObject->xSize;
	this->yOffset = yOffsetPct * this->weaponWieldingObject->ySize;


}



void Weapon::fire()
{

	 //bullet;
	WorldObject* bullet = new WorldObject(this->bulletGameObjectId, 0, 0, 0);

	float dx = this->weaponWieldingObject->physicsBody->GetTransform().p.x + cos(this->weaponWieldingObject->physicsBody->GetAngle() );
	float dy = this->weaponWieldingObject->physicsBody->GetTransform().p.y + sin(this->weaponWieldingObject->physicsBody->GetAngle() );



	//dx = dx + cos(this->weaponWieldingObject->physicsBody->GetAngle());
	//dy = dy + sin(this->weaponWieldingObject->physicsBody->GetAngle());
	//dx = dx + 1;
	//dy = dy + 1;
	
	char buffer[256]; sprintf_s(buffer, "%06f", dx);
	string text(buffer);
	game->debugPanel->addItem("BULLETX", text);

	buffer[256]; sprintf_s(buffer, "%06f", dy);
	string text2(buffer);
	game->debugPanel->addItem("BULLETY", text2);
	



	b2Vec2 positionVector = b2Vec2(dx, dy);

	dx = cos(this->weaponWieldingObject->physicsBody->GetAngle()) * bullet->speed; // make speed configurable
	dy = sin(this->weaponWieldingObject->physicsBody->GetAngle()) * bullet->speed; // Y-component.
	b2Vec2 velocityVector = b2Vec2(dx, dy);

	float angle = this->weaponWieldingObject->physicsBody->GetAngle();
	//cout << "fire location" << positionVector.x << " " << positionVector.y << "\n";
	//cout << "position" << positionVector.x << " " << positionVector.y << "\n";
	//bullet->physicsBody->SetFixedRotation(true);
	bullet->physicsBody->SetTransform(positionVector, angle);
	bullet->physicsBody->SetLinearVelocity(velocityVector);
	bullet->currentAnimationState = "ACTIVE";

	//Add the bullet object to the main gameObject collection
	game->addGameObject(bullet, game->MAIN);

}
