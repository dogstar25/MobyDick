#include "GameObject.h"



#include "GameObjectManager.h"
#include "game.h"
#include "EnumMap.h"

#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"
#include "Util.h"


extern std::unique_ptr<Game> game;

GameObject::~GameObject()
{

	//std::cout << this->m_id << " GameObject Destructor called" << std::endl;

}

GameObject::GameObject(std::string gameObjectType, float xMapPos, float yMapPos, float angleAdjust, Scene* parentScene, int layer, bool cameraFollow, std::string name)
{

	Json::Value definitionJSON;

	//Build components
	definitionJSON = GameObjectManager::instance().getDefinition(gameObjectType)->definitionJSON();

	//Category Id and Object Type
	m_type = gameObjectType;
	m_removeFromWorld = false;

	//Layer
	m_layer = layer;

	//Build the unique id
	m_id = _buildId(gameObjectType, xMapPos, yMapPos);

	//Build the unique name if a name wasnt given
	if (name.empty()) {
		m_name = gameObjectType + "_ANON";
	}
	else {
		m_name = name;
	}

	//Trait tags
	for (Json::Value itrControls : definitionJSON["traitTags"])
	{
		uint32_t trait = game->enumMap()->toEnum(itrControls.asString());
		m_traitTags.set(trait);
	}

	//Set the parent Scene
	m_parentScene = parentScene;
	parentScene->incrementGameObjectCount();

	std::shared_ptr<Component> component{};

	for (Json::Value componentJSON : definitionJSON["components"]){

		int componentType = game->enumMap()->toEnum(componentJSON["id"].asString());

		component = game->componentFactory()->create(definitionJSON, m_name, gameObjectType, parentScene, xMapPos, yMapPos, angleAdjust, componentType);
		component->setParent(this);
		addComponent(component);

	}

	//Set the camera to follow this GameObject
	if (cameraFollow) {
		Camera::instance().setFollowMe(this->name());
	}

}

void GameObject::addTouchingObject(std::shared_ptr<GameObject> touchingObject) 
{

	m_touchingGameObjects[touchingObject->id()] = touchingObject;

}

void GameObject::setParent(GameObject* parentObject)
{

	m_parentObject = parentObject;

}

void GameObject::setPosition(float x, float y)
{

	if (hasComponent(ComponentTypes::PHYSICS_COMPONENT)) {
		b2Vec2 b2Point = {x, y};
		auto b2Position = util::toBox2dPoint(b2Point);
		getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT)->setTransform(b2Position);
	}

	getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT)->setPosition(x, y);

}

void GameObject::setPosition(SDL_FPoint position)
{
	if (hasComponent(ComponentTypes::PHYSICS_COMPONENT)) {
		b2Vec2 b2Point = { position.x, position.y };
		auto b2Position = util::toBox2dPoint(b2Point);
		getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT)->setTransform(b2Position);
	}

	getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT)->setPosition(position);

}

void GameObject::setPosition(b2Vec2 position, float angle)
{
	//-1 means don't apply the angle
	if (angle != -1)
	{

		auto radianAngle = util::degreesToRadians(angle);

		if (hasComponent(ComponentTypes::PHYSICS_COMPONENT)) {
			b2Vec2 b2Point = { position.x, position.y };
			auto b2Position = util::toBox2dPoint(b2Point);
			getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT)->setTransform(b2Position, radianAngle);
		}

		getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT)->setPosition(position, angle);
	}
	else
	{
		if (hasComponent(ComponentTypes::PHYSICS_COMPONENT)) {
			b2Vec2 b2Point = { position.x, position.y };
			auto b2Position = util::toBox2dPoint(b2Point);
			getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT)->setTransform(b2Position);
		}

		getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT)->setPosition(position);
	}

}

void GameObject::setWindowRelativePosition(PositionAlignment windowPosition, float adjustX, float adjustY)
{
	const auto& transforComponent = getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	transforComponent->setWindowRelativePosition(windowPosition);
	transforComponent->setWindowPositionAdjustment({ adjustX, adjustY });

}

void GameObject::setPosition(PositionAlignment windowPosition, float adjustX, float adjustY)
{
	float xMapPos{};
	float yMapPos{};

	auto objectWidth = getSize().x;
	auto objectHeight = getSize().y;

	

	if (windowPosition == PositionAlignment::CENTER) {

		xMapPos = (float)game->gameScreenResolution().x / 2;
		yMapPos = (float)game->gameScreenResolution().y / 2;

	}
	else if (windowPosition == PositionAlignment::TOP_CENTER) {

		xMapPos = (float)game->gameScreenResolution().x / 2;
		yMapPos = (objectHeight / 2);
	}
	else if (windowPosition == PositionAlignment::TOP_LEFT) {

		xMapPos = (objectWidth / 2);
		yMapPos = (objectHeight / 2);
	}
	else if (windowPosition == PositionAlignment::TOP_RIGHT) {

		xMapPos = (float)(game->gameScreenResolution().x - (objectWidth / 2));
		yMapPos = (objectHeight / 2);
	}
	else if (windowPosition == PositionAlignment::CENTER_LEFT) {

		xMapPos = (objectWidth / 2);
		yMapPos = (float)game->gameScreenResolution().y / 2;
	}
	else if (windowPosition == PositionAlignment::CENTER_RIGHT) {

		xMapPos = (float)(game->gameScreenResolution().x - (objectWidth / 2));
		yMapPos = (float)game->gameScreenResolution().y / 2;
	}
	else if (windowPosition == PositionAlignment::BOTTOM_LEFT) {

		xMapPos = (objectWidth / 2);
		yMapPos = (float)(game->gameScreenResolution().y - objectHeight);
	}
	else if (windowPosition == PositionAlignment::BOTTOM_CENTER) {

		xMapPos = (float)(game->gameScreenResolution().x / 2);
		yMapPos = (float)(game->gameScreenResolution().y - objectHeight);
	}
	else if (windowPosition == PositionAlignment::BOTTOM_RIGHT) {

		xMapPos = (float)(game->gameScreenResolution().x - (objectWidth / 2));
		yMapPos = (float)(game->gameScreenResolution().y - objectHeight);
	}

	//Add adjustments
	xMapPos += adjustX;
	yMapPos += adjustY;

	setPosition(xMapPos, yMapPos);


}

void GameObject::update()
{

	if (this->updateDisabled() == false) {
		for (auto& component : m_components)
		{
			if (component && component->isDisabled() == false) {
				component->update();
			}
		}

		//Update touching GameObject
		_updateTouchingObjects();

	}

}

/*
Render this object somewhere other than the GameObjects transformcomponent location
*/
void GameObject::render(SDL_FRect destQuad)
{

	//Render yourself
	getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT)->render(destQuad);

}


void GameObject::render()
{

	if (this->renderDisabled() == false) {
		//Render yourself
		getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT)->render();

		//Render your children
		if (hasComponent(ComponentTypes::CHILDREN_COMPONENT)) {

			getComponent<ChildrenComponent>(ComponentTypes::CHILDREN_COMPONENT)->render();
		}

		//Render your attached inventory items
		if (hasComponent(ComponentTypes::ATTACHMENTS_COMPONENT)) {

			getComponent<AttachmentsComponent>(ComponentTypes::ATTACHMENTS_COMPONENT)->render();
		}

		//If you have an arcade particle emitter then render those particles
		if (hasComponent(ComponentTypes::PARTICLE_COMPONENT)) {

			getComponent<ParticleComponent>(ComponentTypes::PARTICLE_COMPONENT)->render();
		}

		//If you have a composite component, then render the composite pieces
		if (hasComponent(ComponentTypes::COMPOSITE_COMPONENT)) {

			getComponent<CompositeComponent>(ComponentTypes::COMPOSITE_COMPONENT)->render();
		}

		//If you have a hud component, then render the hud items
		if (hasComponent(ComponentTypes::HUD_COMPONENT)) {

			getComponent<HudComponent>(ComponentTypes::HUD_COMPONENT)->render();
		}

		//If you have a vitality component, then render the vitality items
		if (hasComponent(ComponentTypes::VITALITY_COMPONENT)) {

			getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT)->render();
		}

		//If you have a IMGui component, then render it
		if (hasComponent(ComponentTypes::IMGUI_COMPONENT)) {

			getComponent<IMGuiComponent>(ComponentTypes::IMGUI_COMPONENT)->render();
		}

		//If you have aa Action component, then render possible interaction menus
		if (hasComponent(ComponentTypes::ACTION_COMPONENT)) {

			getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT)->render();
		}

		//If you have a Container component, then render the container content items
		if (hasComponent(ComponentTypes::CONTAINER_COMPONENT)) {

			getComponent<ContainerComponent>(ComponentTypes::CONTAINER_COMPONENT)->render();
		}

	}
}

void GameObject::reset()
{

	if (hasComponent(ComponentTypes::POOL_COMPONENT)) {
		getComponent<PoolComponent>(ComponentTypes::POOL_COMPONENT)->reset();
	}

	if (hasComponent(ComponentTypes::PHYSICS_COMPONENT)) {
		getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT)->setOffGrid();
		getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT)->update();
	}


}

void GameObject::addInventoryItem( GameObject* gameObject)
{
	//size_t itemCount = getComponent<InventoryComponent>(ComponentTypes::INVENTORY_COMPONENT)->addItem(gameObject);
	//If this is the only inventory item, then attach it to the player of whatever object this is
	/*if (itemCount == 1)
	{
		getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT)->attachItem(gameObject);
	}*/

}

void GameObject::setPhysicsActive(bool active)
{

	const auto& physicsComponent = getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	if (physicsComponent) {
		physicsComponent->setPhysicsBodyActive(active);
	}

}

void GameObject::setAngleInRadians(float angle)
{

	const auto& physicsComponent = getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	if (physicsComponent) {

		physicsComponent->setAngle(angle);
	}

}

void GameObject::setAngleInDegrees(float angle)
{

	const auto& physicsComponent = getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	if (physicsComponent) {

		angle = util::degreesToRadians(angle);

		physicsComponent->setAngle(angle);
	}

}

float GameObject::getAngle()
{

	const auto& physicsComponent = getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	if (physicsComponent) {

		return physicsComponent->angle();
	}
	else {
		const auto& transformComponent = getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
		if (transformComponent) {
			return util::degreesToRadians(transformComponent->angle());
		}
	}

	return 0;
}

float GameObject::getAngleInDegrees()
{

	const auto& physicsComponent = getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	if (physicsComponent) {

		return util::radiansToDegrees(physicsComponent->angle());
	}
	else {
		const auto& transformComponent = getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
		if (transformComponent) {
			return transformComponent->angle();
		}
	}


	return 0;
}

bool GameObject::holdsDependentGameObjects()
{
	if (hasComponent(ComponentTypes::CHILDREN_COMPONENT) ||
		hasComponent(ComponentTypes::COMPOSITE_COMPONENT) ||
		hasComponent(ComponentTypes::ATTACHMENTS_COMPONENT) ||
		hasComponent(ComponentTypes::INVENTORY_COMPONENT) ||
		hasComponent(ComponentTypes::CONTAINER_COMPONENT)) 
	{
		return true;
	}
	else {
		return false;
	}

}

/*
The postInit function allows for initialization that requires all objects in the game to be 'already' instantiated 
ex. The brainComponent needs all navigation related gameObjects to be built first
OR
For at least all components of a gameObject to be instantiated
ex. The HudComponent requires that the tranform component be instantiated so that it can set some of its properties
*/
void GameObject::postInit()
{

	for (int i = 0; i < ComponentTypes::MAX_COMPONENT_TYPES; i++) {

		if (components()[i]) {

			components()[i]->postInit();

		}
	}

}


void GameObject::setParentScene(Scene* parentScene)
{
	m_parentScene = parentScene;
}


SDL_FPoint GameObject::getCenterPosition()
{

	const auto& transformComponent = getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	return(transformComponent->getCenterPosition());

}

bool GameObject::isAlive()
{

	//Drive from isAlive vs isDead - inanimate objects are never alive
	bool alive{};

	if (hasComponent(ComponentTypes::VITALITY_COMPONENT)) {

		const auto& vitalityComponent = getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);
		alive = !vitalityComponent->isDead();
	}

	return alive;


}

SDL_FPoint GameObject::getOriginalPosition()
{

	const auto& transformComponent = getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	SDL_FPoint position = { transformComponent->originalPosition() .x, transformComponent->originalPosition() .y};
	return(position);

}


SDL_FPoint GameObject::getOriginalTilePosition()
{

 	const auto& transformComponent = getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	SDL_FPoint position = { transformComponent->originalTilePosition().x, transformComponent->originalTilePosition().y };
	return(position);

}

b2Vec2 GameObject::getSize()
{

	const auto& transformComponent = getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	return(transformComponent->size());

}

SDL_FPoint GameObject::getTopLeftPosition()
{

	const auto& transformComponent = getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	return(transformComponent->getTopLeftPosition());

}

std::string GameObject::_buildName(std::string rootName, std::string gameObjectType, bool isDependent)
{
	std::string name{};
	if (isDependent) {
		name = name + rootName + gameObjectType;
		//name = std::format("{}_{:.0f}_{:.0f}_{}", rootName, xMapPos, yMapPos, randomid);
	}
	else {

		name = name + rootName + gameObjectType;

	}

	return name;

}

std::string GameObject::_buildId(std::string id, float xMapPos, float yMapPos)
{
	std::string randomid = util::genRandomId(16);
	auto name = std::format("{}_{:.0f}_{:.0f}_{}", id, xMapPos, yMapPos, randomid);

	return name;

}

bool GameObject::isPointingAt(SDL_FPoint gameObjectPosition)
{

	const std::shared_ptr<PhysicsComponent> physicsComponent = getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	//const std::shared_ptr<PhysicsComponent> referenceObjectPhysicsComponent = gameObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);

	float hostAngleDegrees = util::radiansToDegrees(physicsComponent->angle());
	float orientationAngle = atan2(getCenterPosition().y - gameObjectPosition.y, getCenterPosition().x - gameObjectPosition.x);
	float orientationAngleDegrees = util::radiansToDegrees(orientationAngle);

	if ((hostAngleDegrees - orientationAngleDegrees) >= 140 &&
		(hostAngleDegrees - orientationAngleDegrees) <= 220) {
		return true;
	}
	else {
		return false;
	}


}

bool GameObject::isCompositeEmpty()
{

	if (hasComponent(ComponentTypes::COMPOSITE_COMPONENT) == true) {

		const auto& compositeComponent = getComponent<CompositeComponent>(ComponentTypes::COMPOSITE_COMPONENT);
		if (compositeComponent) {
			return compositeComponent->isCompositeEmpty();
		}
	}

	return false;
}


void GameObject::dispatch(SDL_FPoint destination)
{
	const auto& brainComponent = getComponent<BrainComponent>(ComponentTypes::BRAIN_COMPONENT);
	//brainComponent->dispatch(destination);
}

int GameObject::brainState()
{
	const auto& brainComponent = getComponent<BrainComponent>(ComponentTypes::BRAIN_COMPONENT);

	return brainComponent->state();
}


void GameObject::disableUpdate()
{
	m_stateTags.set(StateTag::disabledUpdate, true);

}
void GameObject::enableUpdate()
{
	m_stateTags.set(StateTag::disabledUpdate, false);

}
void GameObject::disablePhysics()
{
	m_stateTags.set(StateTag::disabledPhysics, true);

	if (hasComponent(ComponentTypes::PHYSICS_COMPONENT) == true) {

		const auto& physicsComponent = getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
		if (physicsComponent) {
			physicsComponent->setPhysicsBodyActive(false);
		}
	}

}

void GameObject::enablePhysics()
{
	m_stateTags.set(StateTag::disabledPhysics, false);

	if (hasComponent(ComponentTypes::PHYSICS_COMPONENT) == true) {

		const auto& physicsComponent = getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
		if (physicsComponent) {
			physicsComponent->setPhysicsBodyActive(true);
		}

	}

}
void GameObject::disableRender()
{
	m_stateTags.set(StateTag::disabledRender, true);
}

void GameObject::enableRender()
{
	m_stateTags.set(StateTag::disabledRender, false);
}

void GameObject::disableCollision()
{
	m_stateTags.set(StateTag::disabledCollision, true);

	if (hasComponent(ComponentTypes::PHYSICS_COMPONENT) == true) {
		const auto& physicsComponent = getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
		for (auto fixture = physicsComponent->physicsBody()->GetFixtureList(); fixture != 0; fixture = fixture->GetNext())
		{
			ContactDefinition* contactDefinition = reinterpret_cast<ContactDefinition*>(fixture->GetUserData().pointer);
			contactDefinition->contactTag = ContactTag::GENERAL_FREE;
		}
	}

}

void GameObject::enableCollision()
{
	m_stateTags.set(StateTag::disabledCollision, false);

	if (hasComponent(ComponentTypes::PHYSICS_COMPONENT) == true) {
		const auto& physicsComponent = getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
		for (auto fixture = physicsComponent->physicsBody()->GetFixtureList(); fixture != 0; fixture = fixture->GetNext())
		{
			ContactDefinition* contactDefinition = reinterpret_cast<ContactDefinition*>(fixture->GetUserData().pointer);
			contactDefinition->contactTag = contactDefinition->saveOriginalContactTag;
		}
	}

}

void GameObject::setCollisionTag( int contactTag)
{

	if (hasComponent(ComponentTypes::PHYSICS_COMPONENT) == true) {
		const auto& physicsComponent = getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
		for (auto fixture = physicsComponent->physicsBody()->GetFixtureList(); fixture != 0; fixture = fixture->GetNext())
		{
			ContactDefinition* contactDefinition = reinterpret_cast<ContactDefinition*>(fixture->GetUserData().pointer);
			contactDefinition->contactTag = contactTag;
		}
	}

}

void GameObject::setColor(SDL_Color color)
{

	const auto& renderComponent = getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT);
	renderComponent->setColor(color);

}

void GameObject::setWeaponColor(SDL_Color color)
{

	const auto& weaponComponent = getComponent<WeaponComponent>(ComponentTypes::WEAPON_COMPONENT);
	weaponComponent->setColor(color);

}

void GameObject::setWeaponForce(float force)
{

	const auto& weaponComponent = getComponent<WeaponComponent>(ComponentTypes::WEAPON_COMPONENT);
	weaponComponent->setForce(force);

}

void GameObject::setCompositePieceLevelCap(int levelCap)
{

	const auto& compositeComponent = getComponent<CompositeComponent>(ComponentTypes::COMPOSITE_COMPONENT);
	for (auto& piece : compositeComponent->pieces()) {

		const auto& vitalityComponent = piece.pieceObject->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);
		vitalityComponent->setLevelCap(levelCap);

	}

}

void GameObject::setBrainSensorSize(int brainSensorSize)
{

	const auto& brainComponent = getComponent<BrainComponent>(ComponentTypes::BRAIN_COMPONENT);
	brainComponent->setSightSensorSize(brainSensorSize);

}

void GameObject::setContainerResapwnTimer(float containerResapwnTimer)
{

	const auto& containerComponent = getComponent<ContainerComponent>(ComponentTypes::CONTAINER_COMPONENT);
	containerComponent->setRefillTimer(containerResapwnTimer);

}

void GameObject::setContainerStartCount(int containerStartCount)
{

	const auto& containerComponent = getComponent<ContainerComponent>(ComponentTypes::CONTAINER_COMPONENT);
	containerComponent->setStartCount(containerStartCount);

}

void GameObject::setContainerCapacity(int containerCapacity)
{

	const auto& containerComponent = getComponent<ContainerComponent>(ComponentTypes::CONTAINER_COMPONENT);
	containerComponent->setCapacity(containerCapacity);

}

void GameObject::setOperatingSound(std::string soundAssetId)
{

	const auto& soundComponent = getComponent<SoundComponent>(ComponentTypes::SOUND_COMPONENT);
	std::optional<SoundItem> sound{};

	sound = soundComponent->getSound("OPERATING_SOUND");
	if (sound.has_value()) {

		sound->soundAssetId = soundAssetId;
	}
	else {

		soundComponent->addSound("OPERATING_SOUND", soundAssetId, false, true);
	}

}

void GameObject::_updateTouchingObjects()
{

	m_touchingGameObjects.clear();

	//If this is a physics GameObject then capture a list of every object that it or its aux sensor is currently touching
	if (this->hasComponent(ComponentTypes::PHYSICS_COMPONENT)) {

		const std::shared_ptr<PhysicsComponent> physicsComponent = this->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);

		for (b2ContactEdge* edge = physicsComponent->physicsBody()->GetContactList(); edge; edge = edge->next)
		{
			b2Contact* contact = edge->contact;

			//One of these fixtures being reported as a contact is the object itself, so we dont care about that one. 
			// We only care about the objects are are not this object itself
			GameObject* contactGameObject = reinterpret_cast<GameObject*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
			GameObject* contactGameObject2 = reinterpret_cast<GameObject*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

			if (contact->IsTouching()) {

				//const auto manifold = contact->GetManifold();
				//manifold->localNormal;

				if (contactGameObject != this && contactGameObject->hasTrait(TraitTag::fragment) == false) {

					auto contactGameObjectSharedPtr = m_parentScene->getGameObject(contactGameObject->id());
					this->addTouchingObject(contactGameObjectSharedPtr.value());
				
				}
				else if (contactGameObject2 != this && contactGameObject2->hasTrait(TraitTag::fragment) == false) {

					auto contactGameObjectSharedPtr = m_parentScene->getGameObject(contactGameObject2->id());
					this->addTouchingObject(contactGameObjectSharedPtr.value());
						
				}

			}
		}
	}


}

SDL_Color GameObject::getColor()
{

	const auto& renderComponent = getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT);
	return renderComponent->color();

}


std::vector<SeenObjectDetails> GameObject::getSeenObjects()
{

	if (hasComponent(ComponentTypes::BRAIN_COMPONENT)) {
		const auto& brain = getComponent<BrainComponent>(ComponentTypes::BRAIN_COMPONENT);
		return brain->seenObjects();
	}
	else {
		return std::vector<SeenObjectDetails>();
	}

}

std::vector<GameObject*> GameObject::getTouchingByTrait(const int trait)
{
	std::vector<GameObject*>touchingObjects{};

	for (auto& gameObject : m_touchingGameObjects) {

		if (gameObject.second.expired() == false && gameObject.second.lock()->hasTrait(trait)) {
			touchingObjects.push_back(gameObject.second.lock().get());
		}

	}

	return touchingObjects;

}