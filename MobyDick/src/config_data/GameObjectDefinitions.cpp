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
	/*gina_64.animationComponent.animations.emplace_back(Animation() = { ANIMATION_IDLE,"TX_GINA_64_IDLE", 0.10F, 1 });
	gina_64.animationComponent.animations.emplace_back(Animation() = { ANIMATION_RUN,"TX_GINA_64_RUN", 0.10F, 4 });*/
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

ProtoObjects& ProtoObjects::instance()
{
	static ProtoObjects singletonInstance;
	return singletonInstance;

}

ProtoObjects::ProtoObjects()
{

	GameObject* newObject = nullptr;
	std::shared_ptr<TransformComponent> transform;
	std::shared_ptr<AnimationComponent> animation;
	//
	//Gina64
	//
	newObject = &protoObjects.emplace("gina64", GameObject()).first->second;

	//Transform
	transform = newObject->addComponent(std::make_shared<TransformComponent>(), ComponentTypes::TRANSFORM_COMPONENT);
	transform->setSize(64, 64);
	transform->setAbsolutePositioning(false);

	//Animation
	animation = newObject->addComponent(std::make_shared<AnimationComponent>(), ComponentTypes::ANIMATION_COMPONENT);
	animation->setDefaultAnimationState(ANIMATION_IDLE);
	animation->animations()[ANIMATION_IDLE] = Animation();


	/*
	
	  "animationComponent": {
				"defaultState": "ANIMATION_IDLE",
				"animations": [
					{
						"state": "ANIMATION_IDLE",
						"textureId": "TX_GINA_64_IDLE",
						"speed": 0.10, //time in seconds for each frame. Lower the faster
						"frames": 1
					},
					{
						"state": "ANIMATION_RUN",
						"textureId": "TX_GINA_64_RUN",
						"speed": 0.10, //time in seconds for each frame. Lower the faster
						"frames": 4
					}
				]
			},
	*/

	//gina64_T




}