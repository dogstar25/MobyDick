#include "RayCastCallBack.h"

#include "GameObject.h"

float RayCastCallBack::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction)
{

	auto test = static_cast<GameObject*>(fixture->GetBody()->GetUserData());



	return -1;
}
