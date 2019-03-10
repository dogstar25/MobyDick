#include "Weapon.h"
#include "GameObject.h"
#include "game.h"
#include <string>



Weapon::Weapon()
{
}


Weapon::~Weapon()
{
}

void Weapon::init(string bulletGameObjectId, GameObject* weaponWieldingObject, float xOffsetPct, float yOffsetPct)
{
	
	this->bulletGameObjectId = bulletGameObjectId;
	this->weaponWieldingObject = weaponWieldingObject;

	//Calulate the actual offest amount using the percentages that were sent in
	this->xOffset = xOffsetPct * this->weaponWieldingObject->definition->xSize;
	this->yOffset = yOffsetPct * this->weaponWieldingObject->definition->ySize;


}



void Weapon::fire()
{

	unique_ptr<GameObject> bullet;

	for (int x = 0; x < 50; x++) {
		bullet = Game::gameObjectManager.buildGameObject(this->bulletGameObjectId, 0, 0, 0);

		float dx = this->weaponWieldingObject->physicsBody->GetTransform().p.x + cos(this->weaponWieldingObject->physicsBody->GetAngle());
		float dy = this->weaponWieldingObject->physicsBody->GetTransform().p.y + sin(this->weaponWieldingObject->physicsBody->GetAngle());

		b2Vec2 positionVector = b2Vec2(dx, dy);

		dx = cos(this->weaponWieldingObject->physicsBody->GetAngle()) * 25; // make speed configurable
		dy = sin(this->weaponWieldingObject->physicsBody->GetAngle()) * 25; // Y-component.
		b2Vec2 velocityVector = b2Vec2(dx, dy);

		//float angle = this->weaponWieldingObject->physicsBody->GetAngle();
		//cout << "fire location" << positionVector.x << " " << positionVector.y << "\n";
		//cout << "position" << positionVector.x << " " << positionVector.y << "\n";
		bullet->physicsBody->SetFixedRotation(true);
		bullet->physicsBody->SetTransform(positionVector, bullet->physicsBody->GetAngle());
		bullet->physicsBody->SetLinearVelocity(velocityVector);
		bullet->currentAnimationState = "ACTIVE";
		Game::gameObjects.push_back(move(bullet));
	}

}
