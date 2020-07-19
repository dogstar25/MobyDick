#include "PhysicsSystem.h"
#include "../game.h"
#include "../GameConfig.h"
#include "../components/TransformComponent.h"
#include "../components/PhysicsComponent.h"

void PhysicsSystem::update()
{

	/*
	Could probably call the box2d step here
	FixMe
	*/

	for (auto& entity : mEntities)
	{
		auto& transform = Game::instance().gameCoordinator().GetComponent<TransformComponent>(entity);
		auto& physics = Game::instance().gameCoordinator().GetComponent<PhysicsComponent>(entity);


		//update the UserData - only once - cant do it in the constructor
		if (physics.m_physicsBody->GetUserData() == nullptr)
		{
			physics.m_physicsBody->SetUserData((Entity*)entity);
		}

		//Transfer the physicsComponent coordinates to the transformComponent
		b2Vec2 convertedPosition{ 0,0 };
		float convertedAngle = util::radiansToDegrees(physics.m_physicsBody->GetAngle());

		convertedPosition.x = physics.m_physicsBody->GetPosition().x * GameConfig::instance().scaleFactor();
		convertedPosition.y = physics.m_physicsBody->GetPosition().y * GameConfig::instance().scaleFactor();

		transform.m_position = convertedPosition;
		transform.m_angle = convertedAngle;

	}


}
