#ifndef CHILDREN_COMPONENT_H
#define CHILDREN_COMPONENT_H

#include <array>
#include <vector>
#include <SDL2/SDL.h>
#include <box2d/box2d.h>

#include <json/json.h>

#include "Component.h"
#include "../Globals.h"


class GameObject;
class TransformComponent;


class ChildrenComponent :  public Component
{
public:
	ChildrenComponent();
	ChildrenComponent(Json::Value definitionJSON);
	~ChildrenComponent();

	void update(std::shared_ptr<GameObject>gameObject) override;
	void renderChildren(); //move to render component?

private:
	int
		m_childCount;
	float
		m_childPadding;
	bool
		m_childPositionRelative;
	std::array<std::vector<std::shared_ptr<GameObject>>, CHILD_POSITIONS> m_childObjects;

	b2Vec2 matchParentRotation(SDL_FRect, SDL_FRect, float); //Move to Transform? Child objects can have a reference to their parent
	b2Vec2 _calcChildPosition(b2Vec2 childSize, 
		int locationSlot, int childNumber, int childCount, SDL_FRect parentPositionRec, float parentAngle);


};

#endif
