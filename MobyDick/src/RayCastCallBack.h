#pragma once

#include <vector>
#include <memory>

#include <box2d/box2d.h>

class GameObject;

struct IntersectionItem
{
	b2Fixture* fixture;
	GameObject* gameObject;
	b2Vec2 intersectPoint;
	b2Vec2 normal;
	float fraction;
};

struct less_than_key
{
	inline bool operator() (const IntersectionItem& item1, const IntersectionItem& item2)
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
	const std::vector<IntersectionItem>& intersectionItems() { return m_intersectionItems; }
private:

	RayCastCallBack() {};
	float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override;
	void sortIntersectionItems();

	std::vector<IntersectionItem> m_intersectionItems;
	


};

