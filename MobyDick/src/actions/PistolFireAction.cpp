#include "PistolFireAction.h"


#include "../GameObject.h"
#include "../GameConfig.h"
#include "../SoundManager.h"

PistolFireAction::PistolFireAction()
{

}

PistolFireAction::~PistolFireAction()
{

}

void PistolFireAction::perform(GameObject* gameObject)
{

	auto& weaponComponent = gameObject->weaponComponent.value();
	b2Vec2 position = {0,0};
	float angle = 0;

	//Sound
	SoundManager::instance().playSound("SFX_LASER_002");

	//If this object is NOT a physics object then we need to divide by the scale factor to convert the X.Y
	if (gameObject->physicsComponent) {
		auto& physicsComponent = gameObject->physicsComponent.value();
		position = physicsComponent->position();
		angle = physicsComponent->angle();
	}
	else {
		auto& transformComponent = gameObject->transformComponent.value();
		position = transformComponent->position();
		position.x /= GameConfig::instance().scaleFactor();
		position.y /= GameConfig::instance().scaleFactor();
		angle = util::degreesToRadians(transformComponent->angle());
	}
	
	weaponComponent->fire(position, angle);

}
