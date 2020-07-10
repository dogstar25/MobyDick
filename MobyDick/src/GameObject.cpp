#include "GameObject.h"

#include "GameObjectManager.h"
#include "EventManager.h"

#include "Level.h"


GameObject::~GameObject()
{

}

GameObject::GameObject(std::string gameObjectId, float xMapPos, float yMapPos, float angleAdjust)
{

	//Game Object Id
	m_id = gameObjectId;
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

	//Animation Component
	if (definitionJSON.isMember("animationComponent"))
	{
		Json::Value componentJSON = definitionJSON["animationComponent"];
		m_AnimationComponent = std::make_shared<AnimationComponent>(definitionJSON, this);

	}
	//Transform Component
	if (definitionJSON.isMember("transformComponent"))
	{
		Json::Value componentJSON = definitionJSON["transformComponent"];
		m_TransformComponent = std::make_shared<TransformComponent>(definitionJSON, xMapPos, yMapPos, angleAdjust, this);

	}
	//Physics Component
	if (definitionJSON.isMember("physicsComponent") && definitionJSON.isMember("transformComponent"))
	{
		Json::Value componentJSON = definitionJSON["physicsComponent"];
		m_PhysicsComponent = std::make_shared<PhysicsComponent>(definitionJSON, xMapPos, yMapPos, angleAdjust, this);

	}
	//Vitality Component
	if (definitionJSON.isMember("vitalityComponent"))
	{
		Json::Value componentJSON = definitionJSON["vitalityComponent"];
		m_VitalityComponent = std::make_shared<VitalityComponent>(definitionJSON, this);

	}

	//Player control Component
	if (definitionJSON.isMember("playerControlComponent"))
	{
		Json::Value componentJSON = definitionJSON["playerControlComponent"];
		m_PlayerControlComponent = std::make_shared<PlayerControlComponent>(definitionJSON, this);

	}

	//Text Component
	if (definitionJSON.isMember("textComponent") && definitionJSON.isMember("transformComponent"))
	{
		Json::Value componentJSON = definitionJSON["textComponent"];
		m_TextComponent = std::make_shared<TextComponent>(gameObjectId, definitionJSON, this);
		
	}

	//Children Component
	if (definitionJSON.isMember("childrenComponent") && definitionJSON.isMember("transformComponent"))
	{
		Json::Value componentJSON = definitionJSON["childrenComponent"];
		m_ChildrenComponent = std::make_shared<ChildrenComponent>(definitionJSON, this);

	}

	//Action Component
	if (definitionJSON.isMember("actionComponent"))
	{
		Json::Value componentJSON = definitionJSON["actionComponent"];
		m_ActionComponent = std::make_shared<ActionComponent>(definitionJSON, this);

	}

	//Render Component - Always Build
	m_RenderComponent = std::make_shared<RenderComponent>(definitionJSON, this);



}

void GameObject::_init()
{
	//std::shared_ptr<GameObject>gameObject = std::make_shared<GameObject>(*this);

	///*if (m_AnimationComponent) {
	//	gameObject->m_components.set(ANIMATION_COMPONENT);
	//	m_AnimationComponent->setGameObject(gameObject);
	//}*/
	//if (m_ActionComponent) {
	//	gameObject->m_components.set(ACTION_COMPONENT);
	//	m_ActionComponent->setGameObject(gameObject);
	//}
	//if (m_AttachmentsComponent) {
	//	gameObject->m_components.set(ATTACHMENTS_COMPONENT);
	//	m_AttachmentsComponent->setGameObject(gameObject);
	//}
	//if (m_ChildrenComponent) {
	//	gameObject->m_components.set(CHILDREN_COMPONENT);
	//	m_ChildrenComponent->setGameObject(gameObject);
	//}
	//if (m_CompositeComponent) {
	//	gameObject->m_components.set(COMPOSITE_COMPONENT);
	//	m_CompositeComponent->setGameObject(gameObject);
	//}
	//if (m_ParticleComponent) {
	//	gameObject->m_components.set(PARTICLE_COMPONENT);
	//	m_ParticleComponent->setGameObject(gameObject);
	//}
	//if (m_PhysicsComponent) {
	//	gameObject->m_components.set(PHYSICS_COMPONENT);
	//	m_PhysicsComponent->setGameObject(gameObject);
	//}
	//if (m_PlayerControlComponent) {
	//	gameObject->m_components.set(PLAYERCONTROL_COMPONENT);
	//	m_PlayerControlComponent->setGameObject(gameObject);
	//}
	//if (m_RenderComponent) {
	//	gameObject->m_components.set(RENDER_COMPONENT);
	//	m_RenderComponent->setGameObject(gameObject);
	//}
	//if (m_TextComponent) {
	//	gameObject->m_components.set(TEXT_COMPONENT);
	//	m_TextComponent->setGameObject(gameObject);
	//}
	//if (m_TransformComponent) {
	//	gameObject->m_components.set(TRANSFORM_COMPONENT);
	//	m_TransformComponent->setGameObject(gameObject);
	//}
	//if (m_VitalityComponent) {
	//	gameObject->m_components.set(VITALITY_COMPONENT);
	//	m_VitalityComponent->setGameObject(gameObject);
	//}
	//if (m_WeaponComponent) {
	//	gameObject->m_components.set(WEAPON_COMPONENT);
	//	m_WeaponComponent->setGameObject(gameObject);
	//}





}

void GameObject::setPosition(b2Vec2 position, float angle)
{
	
	//-1 means dont apply the angle
	if (angle != -1)
	{
		m_TransformComponent->setPosition(position, angle);
	}
	else
	{
		m_TransformComponent->setPosition(position);
	}

}


void GameObject::update()
{

	if (m_TransformComponent) {
		m_TransformComponent->update();
	}
	if (m_PhysicsComponent) {
		m_PhysicsComponent->update();
	}
	if (m_ActionComponent) {
		m_ActionComponent->update();
	}
	if (m_AnimationComponent) {
		m_AnimationComponent->update();
	}
	if (m_RenderComponent) {
		m_RenderComponent->update();
	}
	if (m_TextComponent) {
		m_TextComponent->update();
	}
	if (m_ChildrenComponent) {
		m_ChildrenComponent->update();
	}
	if (m_AttachmentsComponent) {
		m_AttachmentsComponent->update();
	}
	if (m_VitalityComponent) {
		m_VitalityComponent->update();
	}
	if (m_WeaponComponent) {
		m_WeaponComponent->update();
	}
	if (m_CompositeComponent) {
		m_CompositeComponent->update();
	}
	if (m_ParticleComponent) {
		m_ParticleComponent->update();
	}
	if (m_PlayerControlComponent) {
		m_PlayerControlComponent->update();
	}


	////Update the mouse state
	//if (SDL_GetRelativeMouseMode() == SDL_FALSE)
	//{
	//	this->updateMouseState();
	//}

	////This object was clicked, so push whatever event is tied to its onClick event property
	//if (m_mouseState == MOUSE_CLICKED)
	//{
	//	this->onMouseClickEvent();
	//}

	////Loop through any possible child objects and update their
	//// position to reflect parent objects position
	//if (m_definition->hasChildObjects == true)
	//{
	//	updateChildObjects();
	//}

}



void GameObject::render()
{
	SDL_Rect* textureSourceRect=NULL;
	SDL_FRect destRect;
	SDL_Texture* texture=NULL;


	//Render yourself
	m_RenderComponent->render();
		
	//Render your children
	if (m_ChildrenComponent)
	{
		m_ChildrenComponent->renderChildren();
	}


		////test outlining object
		//if (m_definition->isMouseSelectable)
		//{
		//	if (m_mouseState == MOUSE_HOVER)
		//	{
		//		this->onMouseHoverRender();
		//	}
		//	else if (m_mouseState == MOUSE_HOLD)
		//	{
		//		this->onMouseHoldRender();
		//	}
		//	else if (m_mouseState == MOUSE_CLICKED)
		//	{
		//		this->onMouseClickRender();
		//	}
		//}

		////Outline the object if defined
		//if (m_definition->renderOutline)
		//{
		//	outlineObject(2);
		//}

	//}

}


//
//
//void GameObject::onMouseHoverRender()
//{
//
//	outlineObject(6);
//}
//
//void GameObject::onMouseClickRender()
//{
//
//	outlineObject(6);
//
//}
//
//void GameObject::onMouseHoldRender()
//{
//	outlineObject(2);
//}
//
//void GameObject::onMouseClickEvent()
//{
//	std::string* actionCode;
//
//	actionCode = new std::string(m_definition->onClickAction);
//	SDL_Event event;
//	event.user.data1 = static_cast<void*>(actionCode);
//	event.type = SDL_USEREVENT;
//	SDL_PushEvent(&event);
//}
//
//void GameObject::updateMouseState()
//{
//	SDL_FRect gameObjectDrawRect;
//	gameObjectDrawRect = this->getRenderDestRect();
//	bool isHovered = false;
//
//	if (m_definition->isMouseSelectable == true)
//	{
//		//Get Mouse Position
//		int mouseX, mouseY;
//		SDL_GetMouseState(&mouseX, &mouseY);
//
//		//Is mouse over the object
//		if (mouseX >= gameObjectDrawRect.x &&
//			mouseX <= gameObjectDrawRect.x + gameObjectDrawRect.w &&
//			mouseY >= gameObjectDrawRect.y &&
//			mouseY <= gameObjectDrawRect.y + gameObjectDrawRect.h)
//		{
//
//			//was this object clicked?
//			if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
//			{
//
//				//Was this object already in a hold state, meaning user is holding mouse clicked on object
//				if (m_mouseState == MOUSE_HOLD)
//				{
//					//stay in "hold" state while user is holding click on object
//					while (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
//					{
//						SDL_PumpEvents();
//					}
//
//					//User has released mouse so now execute the object onClick event
//					//this->onMouseClick();
//					m_mouseState = MOUSE_CLICKED;
//
//				}
//				else
//				{
//					m_mouseState = MOUSE_HOLD;
//				}
//
//			}
//			else
//			{
//				m_mouseState = MOUSE_HOVER;
//			}
//		}
//		else
//		{
//			m_mouseState = MOUSE_NONE;
//		}
//	}
//
//}
//




