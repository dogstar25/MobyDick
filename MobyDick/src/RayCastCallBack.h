#pragma once

#include <box2d/box2d.h>

class RayCastCallBack : public b2RayCastCallback
{
public:
	RayCastCallBack() {};
	~RayCastCallBack() {};

private:

	float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override;


};

