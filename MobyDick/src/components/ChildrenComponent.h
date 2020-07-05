#ifndef CHILDREN_COMPONENT_H
#define CHILDREN_COMPONENT_H

#include <array>
#include <vector>
#include <SDL2/SDL.h>
#include <box2d/box2d.h>

#include "Component.h"
#include "../Globals.h"
//#include "../GameObject.h"

class GameObject;


class ChildrenComponent :  public Component
{
public:
	ChildrenComponent();
	ChildrenComponent(std::string gameObjectId);
	~ChildrenComponent();

	void update() override;


private:
	int
		m_childCount;
	float
		m_childPadding;
	bool
		m_childPositionRelative;

	std::array<std::vector<std::shared_ptr<GameObject>>, constants::CHILD_POSITIONS> m_childObjects;

	//b2Vec2 calcChildPosition(std::shared_ptr<GameObject>, int, int, int);
	void updateChildObjects();
	void buildChildren();

	b2Vec2 matchParentRotation(SDL_FRect, SDL_FRect, float); //Move to Transform? Child objects can have a reference to their parent
	void renderChildObjects(); //move to render component?


};

#endif
