#ifndef CHILDREN_COMPONENT_H
#define CHILDREN_COMPONENT_H

#include <array>
#include <vector>
#include <SDL2/SDL.h>

#include <box2d/box2d.h>
#include <json/json.h>

#include "Component.h"
#include "../Util.h"
#include "../BaseConstants.h"


class GameObject;
class Scene;
class TransformComponent;


enum class ChildLocationType {

	SLOT,
	ABSOLUTE_POSITION
};

struct ChildLocation {

	ChildLocationType locationType{};
	b2Vec2 absolutePosition{};
	int slot{};
	PositionAlignment positionAlignment{ PositionAlignment::CENTER };
	bool centeredOnLocation{true};
};

struct Child {
	ChildLocation location{};
	std::shared_ptr<GameObject> gameObject{};
};

class ChildrenComponent : public Component
{
public:
	ChildrenComponent();
	ChildrenComponent(Json::Value componentJSON, std::string parentName, Scene* scene);
	~ChildrenComponent();

	void update() override;
	void render();
	void postInit();

	const auto& childObjects() { return m_childObjects; }

private:
	int	  m_childCount {};
	float m_childPadding {};
	bool  m_childPositionRelative {};
	std::array<int, CHILD_POSITIONS> m_childSlotCount{};

	std::vector<Child> m_childObjects;

	b2Vec2 _matchParentRotation(SDL_FPoint childPosition, SDL_FPoint parentPosition, float); //Move to Transform? Child objects can have a reference to their parent
	//b2Vec2 _calcChildPosition(b2Vec2 childSize,
	//	int locationSlot, int locationAlignment, int childNumber, int childCount, SDL_FPoint parentPositionRec, float parentAngle);

	b2Vec2 _calcChildPosition(b2Vec2 childSize, int childCount, ChildLocation location, SDL_FPoint parentPositionRec, float parentAngle);
	b2Vec2 _calcChildPosition(b2Vec2 childSize, ChildLocation location, SDL_FPoint parentPositionRec, float parentAngle);
	std::string _buildChildName(std::string parentName, int childCount);
	void _removeFromWorldPass();


};

#endif
