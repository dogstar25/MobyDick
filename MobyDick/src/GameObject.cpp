#include "GameObject.h"


#include "GameObjectManager.h"
#include "game.h"
#include "SceneManager.h"
#include "Scene.h"
#include "components/InventoryComponent.h"

#include "Globals.h"
#include "Camera.h"
#include "EnumMaps.h"


GameObject::~GameObject()
{

}

GameObject::GameObject(std::string gameObjectId, float xMapPos, float yMapPos, float angleAdjust)
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
	m_idTag = EnumMap::instance().toEnum(definitionJSON["idTag"].asString());
	m_gameObjectType = EnumMap::instance().toEnum(definitionJSON["type"].asString());

	m_removeFromWorld = false;

	//Always build a render and transform component
	addComponent(std::make_shared<RenderComponent>(definitionJSON), ComponentTypes::RENDER_COMPONENT);
	addComponent(std::make_shared<TransformComponent>(definitionJSON, xMapPos, yMapPos, angleAdjust), ComponentTypes::TRANSFORM_COMPONENT);

	//Animation Component
	if (definitionJSON.isMember("animationComponent"))
	{
		addComponent(std::make_shared<AnimationComponent>(definitionJSON), ComponentTypes::ANIMATION_COMPONENT);
	}

	//Physics Component
	if (definitionJSON.isMember("physicsComponent") && definitionJSON.isMember("transformComponent"))
	{
		addComponent(std::make_shared<PhysicsComponent>(definitionJSON, xMapPos, yMapPos, angleAdjust), ComponentTypes::PHYSICS_COMPONENT);
	}

	//Vitality Component
	if (definitionJSON.isMember("vitalityComponent"))
	{
		addComponent(std::make_shared<VitalityComponent>(definitionJSON), ComponentTypes::VITALITY_COMPONENT);
	}

	//Player control Component
	if (definitionJSON.isMember("playerControlComponent"))
	{
		addComponent(std::make_shared<PlayerControlComponent>(definitionJSON), ComponentTypes::PLAYERCONTROL_COMPONENT);
	}

	//Text Component
	if (definitionJSON.isMember("textComponent"))
	{
		addComponent(std::make_shared<TextComponent>(gameObjectId, definitionJSON), ComponentTypes::TEXT_COMPONENT);
	}

	//Children Component
	if (definitionJSON.isMember("childrenComponent"))
	{
		addComponent(std::make_shared<ChildrenComponent>(definitionJSON), ComponentTypes::CHILDREN_COMPONENT);
	}

	//Action Component
	if (definitionJSON.isMember("actionComponent"))
	{
		addComponent(std::make_shared<ActionComponent>(definitionJSON), ComponentTypes::ACTION_COMPONENT);
	}

	//Particle X Component
	if (definitionJSON.isMember("particleXComponent"))
	{
		addComponent(std::make_shared<ParticleXComponent>(definitionJSON), ComponentTypes::PARTICLE_X_COMPONENT);
	}

	//Simple Particle Component
	if (definitionJSON.isMember("particleComponent"))
	{
		addComponent(std::make_shared<ParticleComponent>(definitionJSON), ComponentTypes::PARTICLE_COMPONENT);
	}

	//Inventory Component
	if (definitionJSON.isMember("inventoryComponent"))
	{
		addComponent(std::make_shared<InventoryComponent>(definitionJSON), ComponentTypes::INVENTORY_COMPONENT);
	}

	//UIControl Component
	if (definitionJSON.isMember("UIControlComponent"))
	{
		addComponent(std::make_shared<UIControlComponent>(definitionJSON), ComponentTypes::UICONTROL_COMPONENT);
	}

	//Weapon Component
	if (definitionJSON.isMember("weaponComponent"))
	{
		addComponent(std::make_shared<WeaponComponent>(definitionJSON), ComponentTypes::WEAPON_COMPONENT);
	}

	//Pool Component
	if (definitionJSON.isMember("poolComponent"))
	{
		addComponent(std::make_shared<PoolComponent>(definitionJSON), ComponentTypes::POOL_COMPONENT);
	}

	//Composite Component
	if (definitionJSON.isMember("compositeComponent"))
	{
		addComponent(std::make_shared<CompositeComponent>(definitionJSON), ComponentTypes::COMPOSITE_COMPONENT);
	}

	//Brain Component
	if (definitionJSON.isMember("brainComponent"))
	{
		addComponent(std::make_shared<BrainComponent>(definitionJSON), ComponentTypes::BRAIN_COMPONENT);
	}

	//Navigation Component
	if (definitionJSON.isMember("navigationComponent"))
	{
		addComponent(std::make_shared<NavigationComponent>(definitionJSON), ComponentTypes::NAVIGATION_COMPONENT);
	}

}

//void GameObject::addComponent(int componentId, std::shared_ptr<Component> component)
//{
//
//
//
//
//}


void GameObject::_setDependecyReferences()
{

	//SetRenderComponent dependencies
	//if (hasComponent(ComponentTypes::RENDER_COMPONENT)) {
	//	getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT)->setDependencyReferences(this);
	//}

}

void GameObject::setPosition(float x, float y)
{

	getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT)->setPosition(x, y);

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




void GameObject::render()
{

	//Render yourself
	getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT)->render();
		
	//Render your children
	if (hasComponent(ComponentTypes::CHILDREN_COMPONENT)){

		getComponent<ChildrenComponent>(ComponentTypes::CHILDREN_COMPONENT)->renderChildren();
	}

	//Render your attached inventory items
	if (hasComponent(ComponentTypes::INVENTORY_COMPONENT)) {

		getComponent<InventoryComponent>(ComponentTypes::INVENTORY_COMPONENT)->render();
	}

	//If you have an arcade particle emitter then render those particles
	if (hasComponent(ComponentTypes::PARTICLE_COMPONENT)) {

		getComponent<ParticleComponent>(ComponentTypes::PARTICLE_COMPONENT)->render();
	}

	//If you have a composite component, then render the composite pieces
	if (hasComponent(ComponentTypes::COMPOSITE_COMPONENT)) {

		getComponent<CompositeComponent>(ComponentTypes::COMPOSITE_COMPONENT)->render();
	}


}

//bool GameObject::hasComponent(int componentId) {
//
//	auto iter = m_components.find(componentId);
//
//	if (iter != m_components.end())
//	{
//		if (iter->second && iter->second->isActive() == true)
//		{
//			return true;
//		}
//		else
//		{
//			return false;
//		}
//
//	}
//	else
//	{
//		return false;
//	}
//}
//
//std::shared_ptr<Component> GameObject::getRefComponent(int componentId)
//{
//
//	if (hasComponent(componentId))
//	{
//		return m_components.at(componentId);
//	}
//	else
//	{
//		return std::shared_ptr<Component>();
//	}
//
//}

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
	size_t itemCount = getComponent<InventoryComponent>(ComponentTypes::INVENTORY_COMPONENT)->addItem(gameObject);
	//If this is the only inventory item, then attach it to the player of whatever object this is
	if (itemCount == 1)
	{
		getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT)->attachItem(gameObject);
	}

}

/*
Execute certain initialization that needs to happen AFTER all components have been constructed and
the GameObject has been emplace/constructed into the collection
*/
void GameObject::init(bool cameraFollow)
{

	//Set the root gameObject as the parent for each component
	for (auto& component : m_components) {
		if (component) {
			component->setParent(this);
		}

	}

	//If we specified that the camera follow this gameobject then tell the camera
	if (cameraFollow) {
		Camera::instance().setFollowMe(this);
	}

	//NEW - execute special code for certain extra complicated gameObjects that need to execute after main construction
	// UPDATE_TODD!!!! - can these special things be put into the "update" function to first detect that certain things are not fully initialized
	// and therefore do this "post-creation" initialization?
	if (id() == "DRONE") {

		//Weld Oncomposite pieces
		const auto& droneCompositeComponent = getComponent<CompositeComponent>(ComponentTypes::COMPOSITE_COMPONENT);
		if (droneCompositeComponent->physicsWeldPiecesOn() == true) {
			droneCompositeComponent->weldOnPieces();
		}

	}
	else if (id() == "GINA_64") {

		const auto& playerInventoryComponent = getComponent<InventoryComponent>(ComponentTypes::INVENTORY_COMPONENT);
		playerInventoryComponent->weldOnAttachments();
	}


}

void GameObject::setPhysicsActive(bool active)
{

	const auto& physicsComponent = getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	if (physicsComponent) {
		physicsComponent->setPhysicsBodyActive(active);
	}

}

/*
The postInit function allows gameobjects to initilaizes components that require that ALL gameObjects be built first.
ex. The brainComponent needs all navigation related gameObjects to be built first
*/
void GameObject::postInit(const std::array <std::vector<std::shared_ptr<GameObject>>, MAX_GAMEOBJECT_LAYERS>& gameObjectCollection)
{

	//GameObjects with a NavigationComponent needs to build a navigation array based on the location of 
	//other navigation objects
	if (hasComponent(ComponentTypes::NAVIGATION_COMPONENT)) {
		postInitNavigation(gameObjectCollection);
	}




}

void GameObject::postInitNavigation(const std::array <std::vector<std::shared_ptr<GameObject>>, MAX_GAMEOBJECT_LAYERS>& gameObjectCollection)
{

	const auto& navComponent = getComponent<NavigationComponent>(ComponentTypes::NAVIGATION_COMPONENT);

	//For this GameObject, find all other navigation gameobjects that have been created, 
	// and initilaize the navigation data required
	for (auto& gameObject : gameObjectCollection[LAYER_ABSTRACT])
	{

		if (gameObject->hasComponent(ComponentTypes::NAVIGATION_COMPONENT)) {

			//Ignore the gameObject that IS this particular gameObject
			if (this != gameObject.get()) {


				//Game::instance().physicsWorld()->RayCast(&m_b2RayCastCallback, position, newPosition);
				//SceneManager::instance().scenes().back()


			}



		}



	}




}