#include "Weapon.h"
#include "GameObject.h"
#include "WorldObject.h"
#include "ParticleObject.h"
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
	ParticleObject* bullet = new ParticleObject(this->bulletGameObjectId, 0, 0, 0);
	//ParticleObject* bullet = game->objectPoolManager.get(this->bulletGameObjectId);

	float dx = this->weaponWieldingObject->physicsBody->GetTransform().p.x +
		cos(this->weaponWieldingObject->physicsBody->GetAngle());
	float dy = this->weaponWieldingObject->physicsBody->GetTransform().p.y +
		sin(this->weaponWieldingObject->physicsBody->GetAngle());

	/* remove offfset logic for now*/
	float xAdj = cos(this->weaponWieldingObject->physicsBody->GetAngle()) *(this->xOffset);
	float yAdj = sin(this->weaponWieldingObject->physicsBody->GetAngle()) *(this->yOffset);
	/*
	dx += xAdj;
	dy += yAdj;
	*/

	////////
	// DEBUG VALUES
	game->debugPanel->addItem("PLAYERX", 
		to_string(this->weaponWieldingObject->physicsBody->GetTransform().p.x));
	game->debugPanel->addItem("PLAYERY", 
		to_string(this->weaponWieldingObject->physicsBody->GetTransform().p.y));
	game->debugPanel->addItem("BULLETX", to_string(xAdj));
	game->debugPanel->addItem("BULLETY", to_string(yAdj));
	//////////

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
	///bullet->physicsBody->SetBullet(true);

	//Add the bullet object to the main gameObject collection
	game->addGameObject(bullet, game->MAIN);

}
