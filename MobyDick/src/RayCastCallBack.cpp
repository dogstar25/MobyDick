#include "RayCastCallBack.h"

#include <algorithm>

#include "GameObject.h"

RayCastCallBack& RayCastCallBack::instance()
{
	static RayCastCallBack singletonInstance;
	return singletonInstance;
}


float RayCastCallBack::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction)
{

	IntersectionItem intersectionItem;
	intersectionItem.fixture = fixture;
	intersectionItem.gameObject = static_cast<GameObject*>(fixture->GetBody()->GetUserData());
	intersectionItem.intersectPoint = point;
	intersectionItem.normal = normal;
	intersectionItem.fraction = fraction;

	m_intersectionItems.push_back(intersectionItem);

	return -1;
}

bool RayCastCallBack::hasClearNavPath()
{
	sortIntersectionItems();

	bool obstructionFound = false;

	for (auto intersectionItem : m_intersectionItems) {

		if (intersectionItem.gameObject->hasTrait(TraitTag::barrier)) {

			obstructionFound = true;
			break;
		}
	}

	return !obstructionFound;
}

void RayCastCallBack::reset()
{
	m_intersectionItems.clear();

}

void RayCastCallBack::sortIntersectionItems()
{

	std::sort(m_intersectionItems.begin(), m_intersectionItems.end(), less_than_key());

}
