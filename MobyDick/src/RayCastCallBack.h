#pragma once

#include <vector>
#include <memory>

#include <box2d/box2d.h>

class GameObject;

struct BrainRayCastFoundItem
{
	b2Fixture* fixture;
	GameObject* gameObject;
	b2Vec2 intersectPoint;
	b2Vec2 normal;
	float fraction;
};

struct intersection_sort_compare
{
	inline bool operator() (const BrainRayCastFoundItem& item1, const BrainRayCastFoundItem& item2)
	{
		return (item1.fraction < item2.fraction);
	}
};

class RayCastCallBack : public b2RayCastCallback
{
public:
	static RayCastCallBack& instance();
	
	bool hasClearNavPath();
	void reset();
	std::vector<BrainRayCastFoundItem>& intersectionItems() { return m_intersectionItems; }
private:

	RayCastCallBack() {};
	float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override;
	void sortIntersectionItems();

	std::vector<BrainRayCastFoundItem> m_intersectionItems;
	


};

