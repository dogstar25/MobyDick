#include "BrainAABBCallback.h"
#include "BaseConstants.h"
#include "GameObject.h"


BrainAABBCallback& BrainAABBCallback::instance()
{
	static BrainAABBCallback singletonInstance;
	return singletonInstance;
}


bool BrainAABBCallback::ReportFixture(b2Fixture* fixture)
{
	BrainAABBFoundObject brainAABBFoundObject;
	//brainAABBFoundObject.fixture = fixture;
	brainAABBFoundObject.gameObject = reinterpret_cast<GameObject*>(fixture->GetBody()->GetUserData().pointer);

	//Do not store Particles or other objects that we dont care about
	if (brainAABBFoundObject.gameObject->hasTrait(TraitTag::fragment) == false) {

		m_foundObjects.push_back(brainAABBFoundObject);

	}

	return true;
}

void BrainAABBCallback::reset()
{
	m_foundObjects.clear();

}

