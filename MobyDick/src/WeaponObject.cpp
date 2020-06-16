#include "WeaponObject.h"

#include "GameObject.h"
#include "ParticleObject.h"
#include "ObjectPoolManager.h"
#include "SoundManager.h"
#include "Game.h"

#include <string>


WeaponObject::WeaponObject(string gameObjectId, float xMapPos, float yMapPos, float angleAdjust) :
	WorldObject(gameObjectId, xMapPos, yMapPos, angleAdjust)
{

	m_currentLevel = 1;


}

WeaponObject::~WeaponObject()
{
}

void WeaponObject::fire(const b2Vec2 &origin, const float &angle, const float &fireOffset)
{

	string bulletPoolId = 
		this->definition()->weaponDetails.weaponLevelDetails[m_currentLevel].bulletPoolId;
	
	ParticleObject* bullet = ObjectPoolManager::instance().get(bulletPoolId);
	if (bullet != NULL) {

		//Sound
		SoundManager::instance().playSound("SFX_LASER_002");

		SDL_Color color = this->definition()->weaponDetails.weaponLevelDetails[m_currentLevel].color;
		int strength = this->definition()->weaponDetails.weaponLevelDetails[m_currentLevel].strength;

		//Calculate the origin of the bullet
		float dx = origin.x + cos(angle);
		float dy = origin.y + sin(angle);

		//Calculate offset values of bullet spawning origin adding an offset for the fireing object
		float xAdj = cos(angle) * (fireOffset);
		float yAdj = sin(angle) * (fireOffset);

		dx += xAdj;
		dy += yAdj;

		//Bullet Strength
		bullet->setStrength(strength);

		b2Vec2 positionVector = b2Vec2(dx, dy);

		dx = cos(angle) * bullet->speed(); // make speed configurable
		dy = sin(angle) * bullet->speed(); // Y-component.
		b2Vec2 velocityVector = b2Vec2(dx, dy);

		bullet->physicsBody()->SetFixedRotation(true);
		bullet->physicsBody()->SetTransform(positionVector, angle);
		bullet->physicsBody()->SetLinearVelocity(velocityVector);
		bullet->physicsBody()->SetBullet(true);

		bullet->setColor(color);

		//Add the bullet object to the main gameObject collection
		game->addGameObject(bullet, GameOjectLayer::MAIN);
	}

}

bool WeaponObject::checkLevelUp(const int &ritemCount)
{

	if(m_currentLevel < this->definition()->weaponDetails.weaponLevelDetails.size() &&
		ritemCount >= this->definition()->weaponDetails.weaponLevelDetails[m_currentLevel].levelUpTarget)
	{
		return true;
	}
	else
	{
		return false;
	}

}

void WeaponObject::levelUp()
{
	if (m_currentLevel < this->definition()->weaponDetails.weaponLevelDetails.size())
	{
		m_currentLevel += 1;
	}
	

}

