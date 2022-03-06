#include "TurretFireAction.h"


#include "GameObject.h"
#include "SoundManager.h"

TurretFireAction::TurretFireAction()
{

}

TurretFireAction::~TurretFireAction()
{

}

void TurretFireAction::perform(GameObject* gameObject)
{

	const auto& weaponComponent = gameObject->getComponent<WeaponComponent>(ComponentTypes::WEAPON_COMPONENT);

	SoundManager::instance().playSound("SFX_LASER_002");

	const auto& physicsComponent = gameObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	b2Vec2 position = physicsComponent->position();
	float angle = physicsComponent->angle();
	
	weaponComponent->fire(position, angle);

}
