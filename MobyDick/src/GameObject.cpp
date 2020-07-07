#include "GameObject.h"

#include "GameObjectManager.h"
#include "EventManager.h"

//#include "Level.h"


GameObject::~GameObject()
{

}

GameObject::GameObject(std::string gameObjectId, int xMapPos, int yMapPos, int angleAdjust)
{

	//Game Object Id
	m_id = gameObjectId;

	//Build components
	Json::Value definitionJSON = GameObjectManager::instance().getDefinition(gameObjectId)->definitionJSON();

	//Animation Component
	if (definitionJSON.isMember("animationComponent") && definitionJSON.isMember("transformComponent"))
	{
		m_AnimationComponent = std::make_shared<AnimationComponent>(definitionJSON);

	}
	//Transform Component
	if (definitionJSON.isMember("transformComponent"))
	{
		m_TransformComponent = std::make_shared<TransformComponent>(definitionJSON, xMapPos, yMapPos, angleAdjust);

	}
	//Physics Component
	if (definitionJSON.isMember("physicsComponent") && definitionJSON.isMember("transformComponent"))
	{
		m_PhysicsComponent = std::make_shared<PhysicsComponent>(definitionJSON, xMapPos, yMapPos, angleAdjust);

	}
	//Vitality Component
	if (definitionJSON.isMember("vitalityComponent"))
	{
		m_VitalityComponent = std::make_shared<VitalityComponent>(definitionJSON);

	}

	//Render Component
	m_RenderComponent = std::make_shared<RenderComponent>(definitionJSON);

	//Player control Component
	if (definitionJSON.isMember("playerControlComponent") && definitionJSON.isMember("physicsComponent") && definitionJSON.isMember("vitalityComponent"))
	{
		m_PlayerControlComponent = std::make_shared<PlayerControlComponent>(definitionJSON);

	}

	//Text Component
	if (definitionJSON.isMember("textComponent") && definitionJSON.isMember("transformComponent"))
	{
		m_TextComponent = std::make_shared<TextComponent>(gameObjectId);
		
	}

	//Children Component
	if (definitionJSON.isMember("childrenComponent") && definitionJSON.isMember("transformComponent"))
	{
		m_ChildrenComponent = std::make_shared<ChildrenComponent>(gameObjectId, definitionJSON);

	}

	//Attachments Component

	/*
	Setup component dependency references
	*/
	_setDependecyReferences();




	//is this a debug object then get the default debug definition but change its 
	//id value to the one we passed in
	//if (gameObjectId.rfind("DEBUG_", 0) == 0)
	//{
	//	m_definition = GameObjectManager::instance().gameObjectDefinitions["DEBUG_ITEM"];;
	//}
	//else
	//{
	//	m_definition = GameObjectManager::instance().gameObjectDefinitions[gameObjectId];
	//}



	////Build children if they exist
	//this->buildChildren();

}

void GameObject::_setDependecyReferences()
{
	if (m_AnimationComponent)
	{
		m_AnimationComponent->setDependencyReferences(m_TransformComponent);
	}
	if (m_PhysicsComponent)
	{
		m_PhysicsComponent->setDependencyReferences(m_TransformComponent);
	}
	if (m_PlayerControlComponent)
	{
		m_PlayerControlComponent->setDependencyReferences(m_TransformComponent, m_AnimationComponent, m_PhysicsComponent, m_VitalityComponent);
	}
	if (m_RenderComponent)
	{
		m_RenderComponent->setDependencyReferences(m_TransformComponent, m_AnimationComponent, m_PhysicsComponent);
	}
	if (m_TextComponent)
	{
		m_TextComponent->setDependencyReferences(m_TransformComponent, m_RenderComponent);
	}
	if (m_ChildrenComponent)
	{
		m_ChildrenComponent->setDependencyReferences(m_TransformComponent);
	}

}
void GameObject::setPosition(b2Vec2 position, float angle)
{
	
	if (m_PhysicsComponent)
	{
		b2Vec2 physcisPosition = { 0,0 };

		//Calculate physics position and then call m_physicsComponent.update() to refresh transformComponent

		//-1 means dont apply the angle
		if (angle != -1)
		{
			
		}
		else
		{
			
		}

	}
	else
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


}


void GameObject::update()
{

	if (m_TransformComponent) {
		m_TransformComponent->update();
	}
	if (m_PhysicsComponent) {
		m_PhysicsComponent->update();
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




