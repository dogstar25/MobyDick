#include "GameDefinitions.h"


GameDefs& GameDefs::instance()
{
	static GameDefs singletonInstance;
	return singletonInstance;

}

GameDefs::GameDefs()
{

	/////////////////////////////////
	//GINA_64
	////////////////////////////////
	gina_64.id = "GINA_64";
	//Transform Component
	gina_64.transformComponent.size = { 64,64 };
	gina_64.transformComponent.absolutePositioning = false;
	//Animation Component
	gina_64.animationComponent.animations.emplace_back(Animation() = { ANIMATION_IDLE,"TX_GINA_64_IDLE", 0.10F, 1 });
	gina_64.animationComponent.animations.emplace_back(Animation() = { ANIMATION_RUN,"TX_GINA_64_RUN", 0.10F, 4 });
	//Action Component
	gina_64.actionComponent.actions.emplace_back(ActionItem() = { ACTION_MOVE, new ActorMoveAction() });
	gina_64.actionComponent.actions.emplace_back(ActionItem() = { ACTION_ROTATE, new ActorRotateAction() });
	gina_64.actionComponent.actions.emplace_back(ActionItem() = { ACTION_USE, new ActorUseAction() });
	//Physics Component
	gina_64.physicsComponent = { 
		.b2BodyType = b2_dynamicBody, 
		.b2Shape = b2Shape::e_circle, 
		.radius = 0.64F, .friction = 0.F,
		.restitution = 0.F,	.density = 50.5F, 
		.linearDamping = 0.2F, .angularDamping = 2.0F,
		.collisionCategory = IdTag::PLAYER, 
		.anchorPoint = {.8F, 0.F} };
	/////////////////////////////////
	//SWORDLADY
	////////////////////////////////
	swordlady.id = "SWORDLADY";

}

