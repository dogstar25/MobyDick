#include "RenderComponent.h"

#include "../Renderer.h"
#include "../Camera.h"
#include "../GameObject.h"
#include "../Globals.h"
#include "../GameConfig.h"
#include "../TextureManager.h"
#include "../game.h"

#include "AnimationComponent.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"


RenderComponent::RenderComponent()
{

}


RenderComponent::RenderComponent(Json::Value definitionJSON)
{
	Json::Value itrRender = definitionJSON["renderComponent"];

	if (itrRender.isMember("color"))
	{
		m_color.r = itrRender["color"]["red"].asInt();
		m_color.g = itrRender["color"]["green"].asInt();
		m_color.b = itrRender["color"]["blue"].asInt();
		m_color.a = itrRender["color"]["alpha"].asInt();
	}
	else
	{
		m_color = { 255, 255, 255, 255 };
	}

	m_textureId = itrRender["textureId"].asString();
	m_xRenderAdjustment = itrRender["xRenderAdjustment"].asFloat();
	m_yRenderAdjustment = itrRender["yRenderAdjustment"].asFloat();
	m_renderOutline = itrRender["renderOutline"].asFloat();

	//Get Texture
	m_texture = TextureManager::instance().getTexture(itrRender["textureId"].asString());

}


RenderComponent::~RenderComponent()
{

}


void RenderComponent::setColor(int red, int green, int blue, int alpha)
{
	m_color.r = red;
	m_color.g = green;
	m_color.b = blue;
	m_color.a = alpha;

}
