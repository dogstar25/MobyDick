#include "RenderComponent.h"

#include "../Renderer_SF.h"
#include "../EnumMaps.h"
#include "../Camera.h"
#include "../game.h"


RenderComponent::RenderComponent()
{

}


RenderComponent::RenderComponent(Json::Value definitionJSON)
{
	Json::Value itrRender = definitionJSON["renderComponent"];

	m_gameObjectId = definitionJSON["id"].asString();;

	m_textureId = itrRender["textureId"].asString();
	m_xRenderAdjustment = itrRender["xRenderAdjustment"].asFloat();
	m_yRenderAdjustment = itrRender["yRenderAdjustment"].asFloat();

}


RenderComponent::~RenderComponent()
{

}


void RenderComponent::update()
{

}


void RenderComponent::render()
{
	//Check if this object is in the viewable area of the world
	//Add a tiles width to the camera to buffer it some
	const sf::FloatRect positionRect = parent()->getLocalBounds();

	sf::FloatRect cameraRect = { Camera::instance().frame().x,
		Camera::instance().frame().y,
		Camera::instance().frame().w+ Game::instance().worldTileWidth(),
		Camera::instance().frame().h+ Game::instance().worldTileHeight() };

	//sf::VertexArray test;
	//sf::RenderStates
	//test
	//parent()->draw()

	/*
	If this object is within the viewable are or if its absolute positioned then render it
	*/
	if (positionRect.intersects(cameraRect) ||
		parent()->absolutePositioning() == true) {

		Renderer_SF::instance().window().draw(*parent());
		//parent()->draw(Renderer_SF::instance().window(), sf::RenderStates());

	}

}

void RenderComponent::setDependencyReferences(GameObject* gameObject)
{

	m_animationComponent = gameObject->getComponent<AnimationComponent>().get();
	//m_transformComponent = gameObject->getComponent<TransformComponent>().get();

}

