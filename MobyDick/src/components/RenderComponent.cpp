#include "RenderComponent.h"

#include "../Renderer.h"
#include "../Camera.h"
#include "../GameObjectManager.h"
#include "../GameObject.h"
#include "../Globals.h"
#include "../GameConfig.h"


RenderComponent::RenderComponent()
{

}

/*
Used for GameObject/Component creation
*/
RenderComponent::RenderComponent(RenderComponent* componentDefinition)
{

}

RenderComponent::RenderComponent(std::string gameObjectId, std::shared_ptr<GameObject> parentGameObject)
{
	Json::Value itrJSON = GameObjectManager::instance().getDefinition(gameObjectId)->definitionJSON();

	//Save the pointer to parent GameObject
	m_parentGameObject = parentGameObject;

	//Default values if no component configured
	setColor(255, 255, 255, 255);

	//Transform Component
	if (itrJSON.isMember("renderComponent"))
	{
		m_parentGameObject = parentGameObject;
		m_parentGameObject->setComponentFlag(RENDER_COMPONENT);

		Json::Value itrRender = itrJSON["renderComponent"];

		if (itrRender.isMember("color"))
		{
			setColor(
				itrRender["color"]["red"].asInt(),
				itrRender["color"]["green"].asInt(),
				itrRender["color"]["blue"].asInt(),
				itrRender["color"]["alpha"].asInt()
			);
		}
		else
		{
			setColor(255, 255, 255, 255);
		}

		m_textureId = itrRender["textureId"].asString();
		m_xRenderAdjustment = itrRender["xRenderAdjustment"].asFloat();
		m_yRenderAdjustment = itrRender["yRenderAdjustment"].asFloat();
		m_renderOutline = itrRender["renderOutline"].asFloat();


	}

}


RenderComponent::~RenderComponent()
{

}

void RenderComponent::update()
{

}

/*
Get the destination for rendering the gameObject
*/
SDL_FRect RenderComponent::getRenderDestRect()
{
	SDL_FRect destRect;

	destRect = m_parentGameObject->transformComponent().getPositionRect();

	if (m_parentGameObject->hasComponentFlag(PHYSICS_COMPONENT))
	{
		destRect.w *= GameConfig::instance().scaleFactor();
		destRect.h *= GameConfig::instance().scaleFactor();
		destRect.x *= GameConfig::instance().scaleFactor();
		destRect.y *= GameConfig::instance().scaleFactor();

	}

	//Render Adjustment
	if (m_parentGameObject->hasComponentFlag(PHYSICS_COMPONENT))
	{
		destRect.w += (m_xRenderAdjustment / GameConfig::instance().scaleFactor());
		destRect.h += (m_yRenderAdjustment / GameConfig::instance().scaleFactor());
	}
	else
	{
		destRect.w += m_xRenderAdjustment;
		destRect.h += m_yRenderAdjustment;
	}


	//Adjust position based on current camera position - offset
	destRect.x -= Camera::instance().frame().x;
	destRect.y -= Camera::instance().frame().y;

	return destRect;
}

/*
Get the portion of the gameObject texture to render
For animated objects, this is the portion of the texture that
represents the current frame of animation
*/
SDL_Rect* RenderComponent::getRenderTextureRect()
{
	SDL_Rect* textureSrcRect=nullptr;

	if (m_parentGameObject->hasComponentFlag(ANIMATION_COMPONENT))
	{
		textureSrcRect = m_parentGameObject->animationComponent().getCurrentAnimationTextureRect();
	}

	return textureSrcRect;

}

/*
Get the actual texture to display. If this is an animated object then it will have
different textures for different animation states
*/
SDL_Texture* RenderComponent::getRenderTexture()
{
	SDL_Texture* texture = nullptr;

	if (m_parentGameObject->hasComponentFlag(ANIMATION_COMPONENT)) {

		texture = m_parentGameObject->animationComponent().getCurrentAnimationTexture();
	}
	else {

		texture = m_texture->sdlTexture;
	}

	return texture;

}

SDL_Surface* RenderComponent::getRenderSurface()
{

	return m_texture->surface;

}

void RenderComponent::outlineObject(float lineSize)
{

	std::vector<SDL_FPoint> points;
	SDL_FRect gameObjectDrawRect = getRenderDestRect();
	float saveScaleX, saveScaleY;
	SDL_FPoint point;

	//Adjust for camera
	if (m_parentGameObject->transformComponent().absolutePositioning() == false)
	{
		gameObjectDrawRect.x -= Camera::instance().frame().x;
		gameObjectDrawRect.y -= Camera::instance().frame().y;
	}

	//topleft
	point.x = gameObjectDrawRect.x / lineSize;
	point.y = gameObjectDrawRect.y / lineSize;
	points.push_back(point);

	//topright
	point.x = (gameObjectDrawRect.x + gameObjectDrawRect.w) / lineSize;
	point.y = gameObjectDrawRect.y / lineSize;
	points.push_back(point);

	//bottomright
	point.x = (gameObjectDrawRect.x + gameObjectDrawRect.w) / lineSize;
	point.y = (gameObjectDrawRect.y + gameObjectDrawRect.h) / lineSize;
	points.push_back(point);

	//bottomleft
	point.x = gameObjectDrawRect.x / lineSize;
	point.y = (gameObjectDrawRect.y + gameObjectDrawRect.h) / lineSize;
	points.push_back(point);

	//add the topleft as last point to complete the shape
	point.x = gameObjectDrawRect.x / lineSize;
	point.y = gameObjectDrawRect.y / lineSize;
	points.push_back(point);

	Renderer::instance().outlineObject(points, lineSize);

	points.clear();

}

void RenderComponent::setColor(int red, int green, int blue, int alpha)
{
	m_color.r = red;
	m_color.g = green;
	m_color.b = blue;
	m_color.a = alpha;

}

void RenderComponent::render()
{

	//Get the various items for rendering
	SDL_Rect* textureSourceRect = getRenderTextureRect();
	const SDL_FRect destRect = getRenderDestRect();
	SDL_Texture* texture = getRenderTexture();
	float angle = m_parentGameObject->transformComponent().angle();

	//Set the color
	SDL_SetTextureAlphaMod(texture, m_color.a);

	//SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);

	//Set the render color based on the objects color
	SDL_SetTextureColorMod(texture, m_color.r, m_color.g, m_color.b);

	//Render the texture
	SDL_RenderCopyExF(
		Renderer::instance().SDLRenderer(), 
		texture, 
		textureSourceRect, 
		&destRect, 
		angle, 
		NULL, 
		SDL_FLIP_NONE);

}

//void RenderComponent::render(SDL_FRect* destRect, SDL_Color color)
//{
//
//	//Render the rectangle
//	SDL_SetRenderDrawColor(Renderer::instance().SDLRenderer(), color.r, color.g, color.b, color.a);
//	SDL_RenderFillRectF(Renderer::instance().SDLRenderer(), destRect);
//
//}