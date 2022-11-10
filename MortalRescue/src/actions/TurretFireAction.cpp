#include "TurretFireAction.h"
#include "components/SoundComponent.h"

#include "GameObject.h"
#include "SoundManager.h"

void TurretFireAction::perform(GameObject* gameObject)
{

	const auto& weaponComponent = gameObject->getComponent<WeaponComponent>(ComponentTypes::WEAPON_COMPONENT);

	const auto& physicsComponent = gameObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	b2Vec2 position = physicsComponent->position();
	float angle = physicsComponent->angle();
	
	weaponComponent->fire(position, angle);

	if (gameObject->hasComponent(ComponentTypes::SOUND_COMPONENT) == true) {

		const auto& soundComponent = gameObject->getComponent<SoundComponent>(ComponentTypes::SOUND_COMPONENT);
		soundComponent->playSound("FIRE_SOUND");
	}


}
