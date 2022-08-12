#include "GameObject.h"



#include "GameObjectManager.h"
#include "game.h"
#include "EnumMap.h"

#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"


extern std::unique_ptr<Game> game;

GameObject::~GameObject()
{

//	std::cout << this->m_id << "GameObject Destructor called" << std::endl;

}

GameObject::GameObject(std::string gameObjectId, float xMapPos, float yMapPos, float angleAdjust, Scene* parentScene, int layer, bool cameraFollow, std::string name)
{

	Json::Value definitionJSON;

	//Build components
	definitionJSON = GameObjectManager::instance().getDefinition(gameObjectId)->definitionJSON();

	//Category Id and Object Type
	m_id = gameObjectId;
	m_removeFromWorld = false;

	//Layer
	m_layer = layer;

	//Build the unique name if a name wasnt given
	if (name.empty()) {
		m_name = _buildName(gameObjectId, xMapPos, yMapPos, parentScene);
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

		component = game->componentFactory()->create(definitionJSON, m_name, gameObjectId, parentScene, xMapPos, yMapPos, angleAdjust, componentType);
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

	m_touchingGameObjects[touchingObject->name()] = touchingObject;

}

void GameObject::setParent(GameObject* parentObject)
{

	m_parentObject = parentObject;

}



void GameObject::setPosition(float x, float y)
{

	getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT)->setPosition(x, y);

}

void GameObject::setPosition(SDL_FPoint position)
{

	getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT)->setPosition(position);

}

void GameObject::setPosition(b2Vec2 position, float angle)
{
	//-1 means don't apply the angle
	if (angle != -1)
	{
		getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT)->setPosition(position, angle);
	}
	else
	{
		getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT)->setPosition(position);
	}

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

	return 0;
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

std::string GameObject::_buildName(std::string id, float xMapPos, float yMapPos, Scene* parentScene)
{
	auto name = std::format("{}_{:.0f}_{:.0f}_{:05d}", id, xMapPos, yMapPos, parentScene->gameObjectCount());

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


void GameObject::dispatch(SDL_FPoint destination)
{
	const auto& brainComponent = getComponent<BrainComponent>(ComponentTypes::BRAIN_COMPONENT);
	brainComponent->dispatch(destination);
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

				if (contactGameObject != this) {
					auto contactGameObjectSharedPtr = game->getGameObject(contactGameObject->name());
					if (contactGameObjectSharedPtr.has_value()) {
						this->addTouchingObject(contactGameObjectSharedPtr.value());
					}
				}
				if (contactGameObject2 != this) {
					auto contactGameObjectSharedPtr = game->getGameObject(contactGameObject2->name());
					if (contactGameObjectSharedPtr.has_value()) {
						this->addTouchingObject(contactGameObjectSharedPtr.value());
					}
				}

			}
		}
	}


}