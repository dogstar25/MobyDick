#include "WeaponComponent.h"

#include "../game.h"

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

		WeaponLevelDetail weaponLevelDetail;

		weaponLevelDetail.level = itrWeaponLevel["level"].asInt();
		weaponLevelDetail.levelUpTarget = itrWeaponLevel["levelUpTarget"].asInt();
		weaponLevelDetail.force = itrWeaponLevel["force"].asFloat();
		weaponLevelDetail.color.r = itrWeaponLevel["color"]["red"].asUInt();
		weaponLevelDetail.color.g = itrWeaponLevel["color"]["red"].asUInt();
		weaponLevelDetail.color.b = itrWeaponLevel["color"]["red"].asUInt();
		weaponLevelDetail.color.a = itrWeaponLevel["color"]["red"].asUInt();
		weaponLevelDetail.bulletPoolId = itrWeaponLevel["bulletPoolId"].asString();
		m_weaponLevelDetails.emplace(level, std::move(weaponLevelDetail));

	}


}

WeaponComponent::~WeaponComponent()
{

}

void WeaponComponent::update()
{

}

void WeaponComponent::fire(const b2Vec2& origin, const float& angle)
{
	std::string bulletPoolId =
		m_weaponLevelDetails.at(m_currentLevel).bulletPoolId;
	
	//Get a free bullet
	std::optional<std::shared_ptr<GameObject>> bullet = ObjectPoolManager::instance().getPooledObject(bulletPoolId);

	if(bullet.has_value()){
		//Get references to the bullets components
		auto& vitalityComponent = bullet.value()->getComponent<VitalityComponent>();
		auto& physicsComponent = bullet.value()->getComponent<PhysicsComponent>();
		auto& renderComponent = bullet.value()->getComponent<RenderComponent>();

		SDL_Color color = m_weaponLevelDetails.at(m_currentLevel).color;
		float force = m_weaponLevelDetails.at(m_currentLevel).force;

		//Calculate the origin of the bullet
		float dx = origin.x + cos(angle);
		float dy = origin.y + sin(angle);

		//Calculate offset values of bullet spawning origin adding an offset for the fireing object
		float xAdj = cos(angle) * (m_fireOffset);
		float yAdj = sin(angle) * (m_fireOffset);

		dx += xAdj;
		dy += yAdj;

		//Bullet Strength
		vitalityComponent->setForce(force);

		b2Vec2 positionVector = b2Vec2(dx, dy);

		dx = cos(angle) * vitalityComponent->speed(); // make speed configurable
		dy = sin(angle) * vitalityComponent->speed(); // Y-component.
		b2Vec2 velocityVector = b2Vec2(dx, dy);

		physicsComponent->setFixedRotation(true);
		physicsComponent->setTransform(positionVector, angle);
		physicsComponent->setLinearVelocity(velocityVector);
		physicsComponent->setBullet(true);

		renderComponent->setColor(color);

		//Add the bullet object to the main gameObject collection
		Game::instance().addGameObject(bullet.value(), LAYER_MAIN);
	}

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
