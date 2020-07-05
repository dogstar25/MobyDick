#include "GameObject.h"

#include "GameObjectManager.h"
#include "EventManager.h"

//#include "Level.h"


GameObject::~GameObject()
{

}

void GameObject::init()
{

}

GameObject::GameObject()
	: m_TransformComponent()
{
	this->init();
}

GameObject::GameObject(std::string gameObjectId, float xMapPos, float yMapPos, float angleAdjust) :
	m_AnimationComponent(gameObjectId, std::shared_ptr<GameObject>(this)),
	m_AttachmentsComponent(gameObjectId),
	m_ChildrenComponent(gameObjectId),
	m_CompositeComponent(gameObjectId),
	m_ParticleComponent(gameObjectId),
	m_PhysicsComponent(gameObjectId, std::shared_ptr<GameObject>(this), xMapPos, yMapPos, angleAdjust),
	m_PlayerControlComponent(gameObjectId, std::shared_ptr<GameObject>(this)),
	m_RenderComponent(gameObjectId, std::shared_ptr<GameObject>(this)),
	m_TextComponent(gameObjectId),
	m_TransformComponent(gameObjectId, std::shared_ptr<GameObject>(this), xMapPos, yMapPos, angleAdjust),
	m_VitalityComponent(gameObjectId),
	m_WeaponComponent(gameObjectId)
{

	//Get a pointer to the game object definition
	m_id = gameObjectId;

	////Render Component
	//if (itr.isMember("RenderComponent"))
	//{
	//	m_componentFlags |= RENDER_COMPONENT;
	//	Json::Value& renderComponentJSON = itr["RenderComponent"];
	//	this->m_components.emplace(RENDER_COMPONENT, new RenderComponent(renderComponentJSON));
	//}

	////Animation Component
	//if (itr.isMember("AnimationComponent"))
	//{
	//	m_componentFlags |= ANIMATION_COMPONENT;
	//	Json::Value& animationComponentJSON = itr["AnimationComponent"];
	//	this->m_components.emplace(ANIMATION_COMPONENT, new AnimationComponent(animationComponentJSON));
	//}

	//Need to call the constructor of each compoentn type to build the stuff that wasnt part of
	//getting the component definition






	//calculate position
	//b2Vec2 position(xMapPos * Level::instance().m_tileWidth, yMapPos * Level::instance().m_tileHeight);
	//this->setPosition(position, angleAdjust);

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

	//m_size.Set(m_definition->xSize, m_definition->ySize);

	////color
	//m_color = m_definition->color;

	////Get pointer to the texture
	//m_texture = TextureManager::instance().getTexture(m_definition->textureId);

	////get the animation objects if they exist
	//std::string firstState;
	//int i = 0;
	//if (m_definition->animationDetails.animations.size() > 0)
	//{
	//	Animation* animation = nullptr;
	//	for (auto animationDefinition : m_definition->animationDetails.animations) {

	//		animation = new Animation(m_definition,
	//			animationDefinition.state,
	//			animationDefinition.textureId,
	//			animationDefinition.frames,
	//			animationDefinition.speed);

	//		m_animations.emplace(animationDefinition.state, animation);
	//		//this->animations[animationDefinition.state]= animation;

	//		//Save the first state id 
	//		if (i == 0) {
	//			firstState = animationDefinition.state;
	//		}
	//		++i;
	//	}

	//	//On creation, default the animation state to idle
	//	this->m_currentAnimationState = firstState;
	//}

	////Build children if they exist
	//this->buildChildren();

}

void GameObject::update()
{

	if (hasComponentFlag(TRANSFORM_COMPONENT)) {
		m_TransformComponent.update();
	}
	if (hasComponentFlag(PHYSICS_COMPONENT)) {
		m_PhysicsComponent.update();
	}
	if (hasComponentFlag(ANIMATION_COMPONENT)) {
		m_AnimationComponent.update();
	}
	if (hasComponentFlag(RENDER_COMPONENT)) {
		m_RenderComponent.update();
	}
	if (hasComponentFlag(TEXT_COMPONENT)) {
		m_TextComponent.update();
	}
	if (hasComponentFlag(CHILDREN_COMPONENT)) {
		m_ChildrenComponent.update();
	}
	if (hasComponentFlag(ATTACHMENTS_COMPONENT)) {
		m_AttachmentsComponent.update();
	}
	if (hasComponentFlag(VITALITY_COMPONENT)) {
		m_VitalityComponent.update();
	}
	if (hasComponentFlag(WEAPON_COMPONENT)) {
		m_WeaponComponent.update();
	}
	if (hasComponentFlag(COMPOSITE_COMPONENT)) {
		m_CompositeComponent.update();
	}
	if (hasComponentFlag(PARTICLE_COMPONENT)) {
		m_ParticleComponent.update();
	}
	if (hasComponentFlag(PLAYERCONTROL_COMPONENT)) {
		m_PlayerControlComponent.update();
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


	//if (hasComponentFlag(RENDER_COMPONENT))
	//{

		m_RenderComponent.render();
		

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

		////Loop through any possible child objects, in all 9 positions, and render them too
		//if (m_definition->hasChildObjects == true)
		//{
		//	renderChildObjects();
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

void GameObject::performMovementSequence(float velocity, int direction, int strafe)
{
	//Physics
	m_PhysicsComponent.applyMovement(velocity, direction, strafe);

	//Sound
	playSound(0);

	//Animation
	if (direction == 0 && strafe == 0)
	{
		setAnimationState(ANIMATION_IDLE);
	}
	else
	{
		setAnimationState(ANIMATION_RUN);
	}
}


void GameObject::performRotationSequence(float angularVelocity)
{
	m_PhysicsComponent.applyRotation(angularVelocity);

	//Sound
	playSound(0);

	//Animation
	setAnimationState(ANIMATION_IDLE);

}

void GameObject::playSound(int soundId)
{


}

void GameObject::setAnimationState(int animationState)
{

	m_AnimationComponent.setCurrentAnimationState(animationState);

}


