#pragma once
#include <string>
#include <array>
#include <vector>
#include <bitset>
#include <box2d/box2d.h>

#include "../actions/Action.h"
#include "../actions/ActorMoveAction.h"
#include "../actions/ActorRotateAction.h"
#include "../actions/ActorUseAction.h"
#include "../Globals.h"

using namespace std;




namespace todd
{
	/*
	** COMMON STRUCTS
	*/
	struct Size
	{
		int width;
		int height;
	};

	struct Vec2
	{
		int x;
		int y;
	};

	/*
	*	TRANSFORM COMPONENT
	*/
	struct TransformComponent
	{
		Size size;
		bool absolutePositioning;
	};

	/*
	*	ACTION COMPONENT
	*/
	struct ActionItem
	{
		int		actionId;
		Action* action;
	};

	struct ActionComponent
	{
		vector<ActionItem>	actions;
	};

	/*
		ANIMATION COMPONENT
	*/
	struct Animation
	{
		int		state;
		string	textureId;
		float	speed;
		int		frames;
	};

	struct AnimationComponent
	{
		vector<Animation> animations;
	};

	/*
		PHYSICS COMPONENT
	*/

	struct PysicsComponent
	{
		b2BodyType		b2BodyType;
		b2Shape::Type	b2Shape;
		float			radius;
		float			friction;
		float			restitution;
		float			density;
		float			linearDamping;
		float			angularDamping;
		uint16			collisionCategory;
		Vec2			anchorPoint;
	};

	struct GameObjectDefinition
	{

		string			id;
		//bitset<32>		identityTags;

		TransformComponent	transformComponent;
		AnimationComponent	animationComponent;
		ActionComponent		actionComponent;
		PysicsComponent		physicsComponent;

	};

	vector<GameObjectDefinition> testit =
	{
		{
			"GINA_64",

			//transform component
			{
				{64, 64},
				false
			},
			//Animation Component
			{
				{
					{ ANIMATION_IDLE,"TX_GINA_64_IDLE", 0.10, 1 },
					{ ANIMATION_RUN,"TX_GINA_64_RUN", 0.10, 4 }
				}
			},
			//Action component
			{
				{
					 { ACTION_MOVE, new ActorMoveAction() },
					 { ACTION_ROTATE, new ActorRotateAction() },
					 { ACTION_USE, new ActorUseAction() }
				}
			},
			//Physics component
			//{b2_dynamicBody,b2Shape::e_circle, rad,  fr, re, de,   lin, ang,  COLLISION_PLAYER, anchorPt}
			{  }
		}
	};


	inline void test() {
		GameObjectDefinition test2;
		
		//GINA_64
		test2.id = "GINA_64";
		//Transform Component
		test2.transformComponent.size = { 64,64 };
		test2.transformComponent.absolutePositioning = false;
		//Animation Component
		test2.animationComponent.animations.emplace_back(Animation() = { ANIMATION_IDLE,"TX_GINA_64_IDLE", 0.10, 1});
		test2.animationComponent.animations.emplace_back(Animation() = { ANIMATION_RUN,"TX_GINA_64_RUN", 0.10, 4 });
		//Action Component
		test2.actionComponent.actions.emplace_back(ActionItem() = { ACTION_MOVE, new ActorMoveAction() });
		test2.actionComponent.actions.emplace_back(ActionItem() = { ACTION_ROTATE, new ActorRotateAction() });
		test2.actionComponent.actions.emplace_back(ActionItem() = { ACTION_USE, new ActorUseAction() });
		//Physics Component
		//test2.physicsComponent { b2_dynamicBody, b2Shape::e_circle, rad, fr, re, de, lin, ang,  COLLISION_PLAYER };
		test2.physicsComponent = { b2_dynamicBody, b2Shape::e_circle, 0.64, 0, 0, 50.5, 0.2, 2.0,  COLLISION_PLAYER};
		test2.physicsComponent.anchorPoint = { 1, 1 };
	}

}







