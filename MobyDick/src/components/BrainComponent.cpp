#include "BrainComponent.h"


#include "../game.h"
#include "../RayCastCallBack.h"
#include "../EnumMap.h"



extern std::unique_ptr<Game> game;

BrainComponent::BrainComponent()
{
}

BrainComponent::BrainComponent(Json::Value componentJSON)
{

	m_componentType = ComponentTypes::BRAIN_COMPONENT;

	m_sightSensorSize = componentJSON["sightSensorSize"].asInt();

	for (Json::Value traits : componentJSON["detectObjectTraits"])
	{
		uint32_t trait = game->enumMap()->toEnum(traits.asString());
		m_detectObjectTraits.set(trait);
	}


}

BrainComponent::~BrainComponent()
{

	m_seenObjects.clear();
	m_detectedObjects.clear();

}

void BrainComponent::update()
{

	if (m_updateSensorInputTimer.firstTime == false && m_updateSensorInputTimer.hasMetTargetDuration()) {
		_updateSensorInput();
	}

}

void BrainComponent::postInit()
{
}


void BrainComponent::_updateSensorInput()
{

	auto transform = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	auto centerPosition = parent()->getCenterPosition();

	m_detectedObjects.clear();
	m_seenObjects.clear();

	b2Vec2 centerB2 = { centerPosition.x, centerPosition.y};
	util::toBox2dPoint(centerB2);

	b2AABB aabb;
	aabb.lowerBound = b2Vec2(centerB2.x - m_sightSensorSize, centerB2.y - m_sightSensorSize);
	aabb.upperBound = b2Vec2(centerB2.x + m_sightSensorSize, centerB2.y + m_sightSensorSize);
	
	//Draw the sight sensor
	glm::vec2 topLeft = { aabb.lowerBound.x, aabb.lowerBound.y };
	glm::vec2 topRight = { aabb.upperBound.x, aabb.lowerBound.y };
	glm::vec2 botRight = { aabb.upperBound.x, aabb.upperBound.y };
	glm::vec2 botLeft = { aabb.lowerBound.x, aabb.upperBound.y };
	glm::vec4 lineColor{ 255,255,255,255 };
	util::toRenderPoint(topLeft);
	util::toRenderPoint(topRight);
	util::toRenderPoint(botRight);
	util::toRenderPoint(botLeft);

	//Make the AABB query
	parent()->parentScene()->physicsWorld()->QueryAABB(&BrainAABBCallback::instance(), aabb);

	//Loop through all of the found objects and store the ones that we care about that we have a direct line
	//of sight to
	for (BrainAABBFoundObject detectedObject : BrainAABBCallback::instance().foundObjects()) {

		for (auto i = 0; i < m_detectObjectTraits.size(); i++) {

			//Is this one we care about and is it NOT our own body being detected
			if (detectedObject.gameObject->traits()[i] && m_detectObjectTraits[i]
				&& (detectedObject.gameObject != parent())) {

				//Get this objects shared pointer from the scene and store it instead of the raw pointer
				std::optional<std::weak_ptr<GameObject>> gameObject = parent()->parentScene()->getGameObject(detectedObject.gameObject->id());

				m_detectedObjects.push_back(gameObject.value());

				auto seenObjectDetails = _hasLineOfSight(detectedObject);
				if (seenObjectDetails.has_value() == true) {

					seenObjectDetails.value().gameObject = gameObject.value();
					seenObjectDetails.value().fixture = detectedObject.fixture;
					m_seenObjects.push_back(seenObjectDetails.value());

				}

				break;
			}
		}
	}

	BrainAABBCallback::instance().reset();

}

/*
* This is a ray from a detected object to the Brain Owner
* If no barrier object was found along this ray, then there
* is a clear line of sight
*/
std::optional<SeenObjectDetails> BrainComponent::_hasLineOfSight(BrainAABBFoundObject& detectedObject)
{

	SeenObjectDetails seenObjectDetails{};

	//Get parents position
	auto parentPosition = parent()->getCenterPosition();

	//Cast a ray from this object to the brain parent to see if we have a direct line of sight
	auto objectPosition = detectedObject.gameObject->getCenterPosition();
	b2Vec2 begin = { objectPosition.x, objectPosition.y };
	b2Vec2 end = { parentPosition.x, parentPosition.y };

	util::toBox2dPoint(begin);
	util::toBox2dPoint(end);

	b2Vec2 distance = end - begin;

	bool clearPath{ true };

	// If the distance is zero, 
	//then this object is right on top of the Brain owner so assume its a seen object
	//and dont call the raycast because box2d will blow up if distance is zero
	if (distance.LengthSquared() > 0.0f) {

		//Cast the ray, storing all intersected objects
		parent()->parentScene()->physicsWorld()->RayCast(&RayCastCallBack::instance(), begin, end);

		//Loop through all objects hit between the brain owner and the detected object
		//If there is a clear line of sight then store it in seenObjects
		//We must sort the raycast hit objects by distance because they are not guarenteed to return in
		//distance order
		std::sort(RayCastCallBack::instance().intersectionItems().begin(),
			RayCastCallBack::instance().intersectionItems().end(),
			intersection_sort_compare());

		for (BrainRayCastFoundItem rayHitObject : RayCastCallBack::instance().intersectionItems()) {

			//Is this a barrier and also NOT its own body and the object is not physicsdisabled
			if (rayHitObject.gameObject->hasTrait(TraitTag::barrier) &&
				rayHitObject.gameObject != parent() ) {
				clearPath = false;
				break;
			}

			//If this the brain owning object then store the distance and normal data and break
			if (rayHitObject.gameObject == parent()) {
				
				//save the details but replace the 
				seenObjectDetails.distance = rayHitObject.fraction;
				seenObjectDetails.fixture = rayHitObject.fixture;
				seenObjectDetails.normal = rayHitObject.normal;
				break;
			}
		}

		RayCastCallBack::instance().reset();
	}

	if (clearPath) {
		return seenObjectDetails;
	}
	else {
		return std::nullopt;
	}
}
