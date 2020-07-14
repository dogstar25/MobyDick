#include "WeaponComponent.h"

#include "../GameObjectManager.h"
#include "../GameObject.h"

WeaponComponent::WeaponComponent()
{

}

WeaponComponent::WeaponComponent(Json::Value definitionJSON)
{
	
	Json::Value componentJSON = definitionJSON["weaponComponent"];

	m_currentLevel = 1;

	m_fireOffset = componentJSON["fireOffset"].asFloat();

	for (Json::Value itrWeaponLevel : componentJSON["weaponLevels"])
	{
		int level = itrWeaponLevel["level"].asInt();

		/*std::shared_ptr<WeaponLevelDetail>weaponLevelDetail = std::make_shared< WeaponLevelDetail>();

		weaponLevelDetail->level = itrWeaponLevel["level"].asInt();
		weaponLevelDetail->levelUpTarget = itrWeaponLevel["levelUpTarget"].asInt();
		weaponLevelDetail->strength = itrWeaponLevel["strength"].asInt();
		weaponLevelDetail->color.r = itrWeaponLevel["color"]["red"].asUInt();
		weaponLevelDetail->color.g = itrWeaponLevel["color"]["red"].asUInt();
		weaponLevelDetail->color.b = itrWeaponLevel["color"]["red"].asUInt();
		weaponLevelDetail->color.a = itrWeaponLevel["color"]["red"].asUInt();
		weaponLevelDetail->bulletPoolId = itrWeaponLevel["bulletPoolId"].asString();
		m_weaponLevelDetails.emplace(level, std::move(weaponLevelDetail));*/

	}


}

WeaponComponent::~WeaponComponent()
{

}

void WeaponComponent::update(std::shared_ptr<GameObject>gameObject)
{

}

void WeaponComponent::fire(const b2Vec2& origin, const float& angle, const float& fireOffset)
{
	//std::string bulletPoolId =
	//	this->definition()->weaponDetails.weaponLevelDetails[m_currentLevel].bulletPoolId;

	//ParticleObject* bullet = ObjectPoolManager::instance().getParticle(bulletPoolId);
	//if (bullet != NULL) {

	//	//Sound
	//	SoundManager::instance().playSound("SFX_LASER_002");

	//	SDL_Color color = this->definition()->weaponDetails.weaponLevelDetails[m_currentLevel].color;
	//	int strength = this->definition()->weaponDetails.weaponLevelDetails[m_currentLevel].strength;

	//	//Calculate the origin of the bullet
	//	float dx = origin.x + cos(angle);
	//	float dy = origin.y + sin(angle);

	//	//Calculate offset values of bullet spawning origin adding an offset for the fireing object
	//	float xAdj = cos(angle) * (fireOffset);
	//	float yAdj = sin(angle) * (fireOffset);

	//	dx += xAdj;
	//	dy += yAdj;

	//	//Bullet Strength
	//	bullet->setStrength(strength);

	//	b2Vec2 positionVector = b2Vec2(dx, dy);

	//	dx = cos(angle) * bullet->speed(); // make speed configurable
	//	dy = sin(angle) * bullet->speed(); // Y-component.
	//	b2Vec2 velocityVector = b2Vec2(dx, dy);

	//	bullet->physicsBody()->SetFixedRotation(true);
	//	bullet->physicsBody()->SetTransform(positionVector, angle);
	//	bullet->physicsBody()->SetLinearVelocity(velocityVector);
	//	bullet->physicsBody()->SetBullet(true);

	//	bullet->setColor(color);

	//	//Add the bullet object to the main gameObject collection
	//	Game::instance().addGameObject(bullet, GameOjectLayer::MAIN);
	//}

}

bool WeaponComponent::checkLevelUp(const int& pieceCount)
{
	/*if (m_currentLevel < this->definition()->weaponDetails.weaponLevelDetails.size() &&
		ritemCount >= this->definition()->weaponDetails.weaponLevelDetails[m_currentLevel].levelUpTarget)
	{
		return true;
	}
	else
	{
		return false;
	}*/

	return true;
}

void WeaponComponent::levelUp()
{
	/*if (m_currentLevel < this->definition()->weaponDetails.weaponLevelDetails.size())
	{
		m_currentLevel += 1;
	}*/

}
