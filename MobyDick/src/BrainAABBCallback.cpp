#include "BrainAABBCallback.h"


BrainAABBCallback& BrainAABBCallback::instance()
{
	static BrainAABBCallback singletonInstance;
	return singletonInstance;
}


bool BrainAABBCallback::ReportFixture(b2Fixture* fixture)
{
	BrainAABBFoundObject brainAABBFoundObject;
	brainAABBFoundObject.fixture = fixture;
	brainAABBFoundObject.gameObject = reinterpret_cast<GameObject*>(fixture->GetBody()->GetUserData().pointer);

	//only store the objects that we care about
	//if (brainAABBFoundObject.gameObject->hasTrait(TraitTag::player) ||
	//	brainAABBFoundObject.gameObject->hasTrait(TraitTag::navigation)) {

	//	m_foundObjects.push_back(brainAABBFoundObject);

	//}

	m_foundObjects.push_back(brainAABBFoundObject);

	return true;
}

void BrainAABBCallback::reset()
{
	m_foundObjects.clear();

}

