#pragma once
#include <string>
#include <array>
#include <vector>
#include <bitset>
#pragma warning(push,0)
#include <box2d/box2d.h>
#pragma warning(pop)

#include "../actions/Action.h"
#include "../actions/ActorMoveAction.h"
#include "../actions/ActorRotateAction.h"
#include "../actions/ActorUseAction.h"
#include "../Globals.h"

using namespace std;

/*
** COMMON STRUCTS
*/
namespace GameObjectDefinitionStructures {
	struct Size
	{
		int width{ 0 };
		int height{ 0 };
	};

	struct Vec2
	{
		int x{ 0 };
		int y{ 0 };
	};

	struct Vec2_f
	{
		float x{ 0.0 };
		float y{ 0.0 };
	};

	/*
	*	TRANSFORM COMPONENT
	*/
	struct Transform_Component
	{
		Size size;
		bool absolutePositioning;
	};

	/*
	*	ACTION COMPONENT
	*/
	struct ActionItem
	{
		int		actionId{ 0 };
		Action* action{ nullptr };
	};

	struct Action_Component
	{
		vector<ActionItem>	actions;
	};

	/*
		ANIMATION COMPONENT
	*/
	struct Animation
	{
		int		state{ 0 };
		string	textureId{ "" };
		float	speed{ 0.0 };
		int		frames{ 0 };
	};

	struct Animation_Component
	{
		vector<Animation> animations;
	};

	/*
		PHYSICS COMPONENT
	*/

	struct Pysics_Component
	{
		int				b2BodyType;
		int				b2Shape;
		float			radius;
		float			friction;
		float			restitution;
		float			density;
		float			linearDamping;
		float			angularDamping;
		int				collisionCategory;
		Vec2_f			anchorPoint;
	};

}

using namespace GameObjectDefinitionStructures;

struct GameObjectDef
{


	string			id;
	//bitset<32>		identityTags;

	Transform_Component	transformComponent;
	Animation_Component	animationComponent;
	Action_Component	actionComponent;
	Pysics_Component	physicsComponent;

};

struct GameDefs {

	static GameDefs& instance();
	GameDefs();

	GameObjectDef gina_64;
	GameObjectDef swordlady;


};








