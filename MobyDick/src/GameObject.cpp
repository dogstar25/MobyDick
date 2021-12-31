#include "GameObject.h"


#include <format>

#include "GameObjectManager.h"
#include "game.h"
#include "SceneManager.h"
#include "Scene.h"
#include "components/InventoryComponent.h"
#include "ComponentFactory.h"
#include "Util.h"
#include "BaseConstants.h"
#include "Camera.h"
#include "EnumMaps.h"

#include <typeinfo>

extern std::unique_ptr<Game> game;

GameObject::~GameObject()
{

}

GameObject::GameObject(std::string gameObjectId, float xMapPos, float yMapPos, float angleAdjust, Scene* parentScene, int layer, bool cameraFollow)
{

	Json::Value definitionJSON;

	//Build components
	if (gameObjectId.rfind("DEBUG_", 0) != 0)
	{
		definitionJSON = GameObjectManager::instance().getDefinition(gameObjectId)->definitionJSON();
	}
	else
	{
		definitionJSON = GameObjectManager::instance().getDefinition("DEBUG_ITEM")->definitionJSON();
	}

	//Category Id and Object Type
	m_id = gameObjectId;
	m_collisionTag = EnumMap::instance().toEnum(definitionJSON["collisionTag"].asString());
	m_removeFromWorld = false;

	//Build the unique name
	m_name = _buildName(gameObjectId, xMapPos, yMapPos, parentScene);

	if (gameObjectId.compare("GINA_64") == 0) {
		int todd = 1;
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

	//Always build a render component
	component = game->componentFactory()->create(definitionJSON, layer, ComponentTypes::RENDER_COMPONENT);
	component->setParent(this);
	addComponent(component, ComponentTypes::RENDER_COMPONENT);

	//Always build a transform component
	component = game->componentFactory()->create(definitionJSON, xMapPos, yMapPos, angleAdjust, ComponentTypes::TRANSFORM_COMPONENT);
	component->setParent(this);
	addComponent(component, ComponentTypes::TRANSFORM_COMPONENT);

	//Animation Component
	if (definitionJSON.isMember("animationComponent"))
	{
		component = game->componentFactory()->create(definitionJSON, ComponentTypes::ANIMATION_COMPONENT);
		component->setParent(this);
		addComponent(component, ComponentTypes::ANIMATION_COMPONENT);

	}

	//Physics Component
	if (definitionJSON.isMember("physicsComponent"))
	{

		component = game->componentFactory()->create(definitionJSON, parentScene, xMapPos, yMapPos, angleAdjust, ComponentTypes::PHYSICS_COMPONENT);
		component->setParent(this);
		//std::static_pointer_cast<PhysicsComponent>(component)->physicsBody()->SetUserData((void*)this);
		std::static_pointer_cast<PhysicsComponent>(component)->physicsBody()->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
		addComponent(component, ComponentTypes::PHYSICS_COMPONENT);

	}

	//Vitality Component
	if (definitionJSON.isMember("vitalityComponent"))
	{
		component = game->componentFactory()->create(definitionJSON, ComponentTypes::VITALITY_COMPONENT);
		component->setParent(this);
		addComponent(component, ComponentTypes::VITALITY_COMPONENT);

	}

	//Player control Component
	if (definitionJSON.isMember("playerControlComponent"))
	{
		component = game->componentFactory()->create(definitionJSON, ComponentTypes::PLAYERCONTROL_COMPONENT);
		component->setParent(this);
		addComponent(component, ComponentTypes::PLAYERCONTROL_COMPONENT);

	}

	//Text Component
	if (definitionJSON.isMember("textComponent"))
	{
		component = game->componentFactory()->create(definitionJSON, gameObjectId, ComponentTypes::TEXT_COMPONENT);
		component->setParent(this);
		addComponent(component, ComponentTypes::TEXT_COMPONENT);

	}

	//Children Component
	if (definitionJSON.isMember("childrenComponent"))
	{
		component = game->componentFactory()->create(definitionJSON, parentScene, ComponentTypes::CHILDREN_COMPONENT);
		component->setParent(this);
		addComponent(component, ComponentTypes::CHILDREN_COMPONENT);

	}

	//Action Component
	if (definitionJSON.isMember("actionComponent"))
	{
		component = game->componentFactory()->create(definitionJSON, ComponentTypes::ACTION_COMPONENT);
		component->setParent(this);
		addComponent(component, ComponentTypes::ACTION_COMPONENT);

	}

	//Particle X Component
	if (definitionJSON.isMember("particleXComponent"))
	{
		component = game->componentFactory()->create(definitionJSON, ComponentTypes::PARTICLE_X_COMPONENT);
		component->setParent(this);
		addComponent(component, ComponentTypes::PARTICLE_X_COMPONENT);

	}

	//Simple Particle Component
	if (definitionJSON.isMember("particleComponent"))
	{
		component = game->componentFactory()->create(definitionJSON, ComponentTypes::PARTICLE_COMPONENT);
		component->setParent(this);
		addComponent(component, ComponentTypes::PARTICLE_COMPONENT);

	}

	//Inventory Component
	if (definitionJSON.isMember("inventoryComponent"))
	{
		component = game->componentFactory()->create(definitionJSON, parentScene, ComponentTypes::INVENTORY_COMPONENT);
		component->setParent(this);
		addComponent(component, ComponentTypes::INVENTORY_COMPONENT);

	}

	//UIControl Component
	if (definitionJSON.isMember("UIControlComponent"))
	{
		component = game->componentFactory()->create(definitionJSON, ComponentTypes::UICONTROL_COMPONENT);
		component->setParent(this);
		addComponent(component, ComponentTypes::UICONTROL_COMPONENT);

	}

	//Weapon Component
	if (definitionJSON.isMember("weaponComponent"))
	{
		component = game->componentFactory()->create(definitionJSON, ComponentTypes::WEAPON_COMPONENT);
		component->setParent(this);
		addComponent(component, ComponentTypes::WEAPON_COMPONENT);

	}

	//Pool Component
	if (definitionJSON.isMember("poolComponent"))
	{
		component = game->componentFactory()->create(definitionJSON, ComponentTypes::POOL_COMPONENT);
		component->setParent(this);
		addComponent(component, ComponentTypes::POOL_COMPONENT);

	}

	//Composite Component
	if (definitionJSON.isMember("compositeComponent"))
	{
		component = game->componentFactory()->create(definitionJSON, parentScene, ComponentTypes::COMPOSITE_COMPONENT);
		component->setParent(this);
		addComponent(component, ComponentTypes::COMPOSITE_COMPONENT);

	}

	//Brain Component
	if (definitionJSON.isMember("brainComponent"))
	{
		component = game->componentFactory()->create(definitionJSON, parentScene, ComponentTypes::BRAIN_COMPONENT);
		component->setParent(this);
		addComponent(component, ComponentTypes::BRAIN_COMPONENT);
	}

	//Navigation Component
	if (definitionJSON.isMember("navigationComponent"))
	{
		component = game->componentFactory()->create(definitionJSON, ComponentTypes::NAVIGATION_COMPONENT);
		component->setParent(this);
		addComponent(component, ComponentTypes::NAVIGATION_COMPONENT);
	}

	//Attachments Component
	if (definitionJSON.isMember("attachmentsComponent"))
	{
		component = game->componentFactory()->create(definitionJSON, parentScene, ComponentTypes::ATTACHMENTS_COMPONENT);
		component->setParent(this);
		addComponent(component, ComponentTypes::ATTACHMENTS_COMPONENT);
	}

	//Hud Component
	if (definitionJSON.isMember("hudComponent"))
	{
		component = game->componentFactory()->create(definitionJSON, parentScene, ComponentTypes::HUD_COMPONENT);
		component->setParent(this);
		addComponent(component, ComponentTypes::HUD_COMPONENT);
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
		if (component) {
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

	//Render yourself
	getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT)->render();
		
	//Render your children
	if (hasComponent(ComponentTypes::CHILDREN_COMPONENT)){

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

}

void GameObject::reset()
{
	if (hasComponent(ComponentTypes::POOL_COMPONENT)) {
		getComponent<PoolComponent>(ComponentTypes::POOL_COMPONENT)->reset();
	}
	if (hasComponent(ComponentTypes::PHYSICS_COMPONENT)) {
		getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT)->setOffGrid();
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

	//GameObjects with a NavigationComponent need to build a navigation array based on the location of 
	//other navigation objects
	if (hasComponent(ComponentTypes::NAVIGATION_COMPONENT)) {
		const auto navigationComponent = getComponent<NavigationComponent>(ComponentTypes::NAVIGATION_COMPONENT);
		navigationComponent->postInit();
	}

	//The BrainComponent needs to gather and store all of the waypoint nav points and
	//interim nav points for its navigating logic
	if (hasComponent(ComponentTypes::BRAIN_COMPONENT)) {
		const auto brainComponent = getComponent<BrainComponent>(ComponentTypes::BRAIN_COMPONENT);
		brainComponent->postInit();
	}

	//The CompositeComponent needs to weld on its composite pieces if the weld flag is turned on
	if (hasComponent(ComponentTypes::COMPOSITE_COMPONENT)) {
		const auto compositeComponent = getComponent<CompositeComponent>(ComponentTypes::COMPOSITE_COMPONENT);
		compositeComponent->postInit();
	}

	//The Attachments component needs to weld on attachment objects as well as potentially add the 
	// attachement object to the inventory component
	if (hasComponent(ComponentTypes::ATTACHMENTS_COMPONENT)) {
		const auto attachmentsComponent = getComponent<AttachmentsComponent>(ComponentTypes::ATTACHMENTS_COMPONENT);
		attachmentsComponent->postInit();
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