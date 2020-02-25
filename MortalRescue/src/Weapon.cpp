#include "Weapon.h"

#include "GameObject.h"
#include "ParticleObject.h"
#include "ObjectPoolManager.h"
#include "Game.h"

#include <string>


Weapon::Weapon(string bulletPoolId, float strength, int levelUpTarget, int level)
{

	m_bulletPoolId = bulletPoolId;
	m_strength = strength;
	m_levelUpTarget = levelUpTarget;
	m_level = level;

}


Weapon::~Weapon()
{
}



void Weapon::fire(const b2Vec2 &origin, const float &angle, const float &fireOffset)
{

	ParticleObject* bullet = ObjectPoolManager::instance().get(m_bulletPoolId);
	if (bullet != NULL) {

		//Calculate the origin of the bullet
		float dx = origin.x + cos(angle);
		float dy = origin.y + sin(angle);

		//Calculate offset values of bullet spawning origin adding an offset for the fireing object
		float xAdj = cos(angle) * (fireOffset);
		float yAdj = sin(angle) * (fireOffset);

		dx += xAdj;
		dy += yAdj;

		//Bullet Strength
		bullet->setStrength( m_strength);

		b2Vec2 positionVector = b2Vec2(dx, dy);

		dx = cos(angle) * bullet->speed(); // make speed configurable
		dy = sin(angle) * bullet->speed(); // Y-component.
		b2Vec2 velocityVector = b2Vec2(dx, dy);

		bullet->physicsBody()->SetFixedRotation(true);
		bullet->physicsBody()->SetTransform(positionVector, angle);
		bullet->physicsBody()->SetLinearVelocity(velocityVector);
		bullet->physicsBody()->SetBullet(true);

		bullet->setColor(255,255,255,255);

		//Add the bullet object to the main gameObject collection
		game->addGameObject(bullet, game->MAIN);
	}

}

bool Weapon::checkLevelUp(const int &rPieceCount)
{

	if (rPieceCount >= m_levelUpTarget)
	{
		return true;
	}
	else
	{
		return false;
	}

}

int Weapon::getNextLevel()
{

	return m_level += 1;;

}

