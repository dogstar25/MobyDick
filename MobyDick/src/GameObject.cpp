#include "GameObject.h"



#include "GameObjectManager.h"
#include "game.h"
#include "EnumMaps.h"


extern std::unique_ptr<Game> game;

GameObject::~GameObject()
{

}

GameObject::GameObject(std::string gameObjectId, float xMapPos, float yMapPos, float angleAdjust, Scene* parentScene, bool cameraFollow, std::string name)
{

	Json::Value definitionJSON;

	//Build components
	definitionJSON = GameObjectManager::instance().getDefinition(gameObjectId)->definitionJSON();

	//Category Id and Object Type
	m_id = gameObjectId;
	m_collisionTag = m_originalCollisionTag = EnumMap::instance().toEnum(definitionJSON["collisionTag"].asString());
	m_removeFromWorld = false;

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
		uint32_t trait = EnumMap::instance().toEnum(itrControls.asString());
		m_traitTags.set(trait);
	}

	//Set the parent Scene
	m_parentScene = parentScene;
	parentScene->incrementGameObjectCount();

	std::shared_ptr<Component> component{};

	for (Json::Value componentJSON : definitionJSON["components"]){

		int componentType = EnumMap::instance().toEnum(componentJSON["id"].asString());

		component = game->componentFactory()->create(definitionJSON, m_name, gameObjectId, parentScene, xMapPos, yMapPos, angleAdjust, componentType);
		component->setParent(this);
		addComponent(component, componentType);

	}

	//Set the camera to follow this GameObject
	if (cameraFollow) {
		Camera::instance().setFollowMe(this);
	}

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


void GameObject::update()
{

	for (auto& component : m_components)
	{
		if (component && component->isDisabled() == false) {
			component->update();
		}
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

	if (m_disabled == false) {
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

		//If you have aa Action component, then render possible interaction hints
		if (hasComponent(ComponentTypes::ACTION_COMPONENT)) {

			getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT)->render();
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

/*
The postInit function allows for initialization that requires all objects to be 'already' instantiated 
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


	////GameObjects with a NavigationComponent need to build a navigation array based on the location of 
	////other navigation objects
	//if (hasComponent(ComponentTypes::NAVIGATION_COMPONENT)) {
	//	const auto navigationComponent = getComponent<NavigationComponent>(ComponentTypes::NAVIGATION_COMPONENT);
	//	navigationComponent->postInit();
	//}

	////The BrainComponent needs to gather and store all of the waypoint nav points and
	////interim nav points for its navigating logic
	//if (hasComponent(ComponentTypes::BRAIN_COMPONENT)) {
	//	const auto brainComponent = getComponent<BrainComponent>(ComponentTypes::BRAIN_COMPONENT);
	//	brainComponent->postInit();
	//}

	////The CompositeComponent needs to weld on its composite pieces if the weld flag is turned on
	//if (hasComponent(ComponentTypes::COMPOSITE_COMPONENT)) {
	//	const auto compositeComponent = getComponent<CompositeComponent>(ComponentTypes::COMPOSITE_COMPONENT);
	//	compositeComponent->postInit();
	//}

	////The Attachments component needs to weld on attachment objects as well as potentially add the 
	//// attachement object to the inventory component
	//if (hasComponent(ComponentTypes::ATTACHMENTS_COMPONENT)) {
	//	const auto attachmentsComponent = getComponent<AttachmentsComponent>(ComponentTypes::ATTACHMENTS_COMPONENT);
	//	attachmentsComponent->postInit();
	//}


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

void GameObject::disable(bool disablePhysicsBody)
{
	m_disabled = true;
	if (disablePhysicsBody == true) {

		if (hasComponent(ComponentTypes::PHYSICS_COMPONENT) == true) {
			const auto& physicsComponent = getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
			if (physicsComponent) {
				physicsComponent->setPhysicsBodyActive(false);
			}
		}

	}

}

void GameObject::enable()
{
	m_disabled = false;
	if (hasComponent(ComponentTypes::PHYSICS_COMPONENT) == true) {

		const auto& physicsComponent = getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
		if (physicsComponent) {
			physicsComponent->setPhysicsBodyActive(true);
		}

	}

}