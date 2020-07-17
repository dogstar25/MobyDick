#include "RenderComponent.h"

#include "../Renderer.h"
#include "../Camera.h"
#include "../GameObjectManager.h"
#include "../GameObject.h"
#include "../Globals.h"
#include "../GameConfig.h"
#include "../TextureManager.h"

#include "AnimationComponent.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"


RenderComponent::RenderComponent()
{

}


RenderComponent::RenderComponent(Json::Value definitionJSON)
{
	Json::Value itrRender = definitionJSON["renderComponent"];

	m_gameObjectId = definitionJSON["id"].asString();;

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

	//Get Texture
	m_texture = TextureManager::instance().getTexture(itrRender["textureId"].asString());

}


RenderComponent::~RenderComponent()
{

}


void RenderComponent::update(std::shared_ptr<GameObject>gameObject)
{

}

/*
Get the destination for rendering the gameObject
The end result should be a rectangle with a width and height in pixels and
an x, y position that is the top left corner of the object (for SDL render function)
*/
SDL_FRect RenderComponent::getRenderDestRect(std::shared_ptr<GameObject>gameObject)
{
	SDL_FRect destRect, currentPositionRect;

	//Get its current position. Should be center of object
	currentPositionRect = gameObject->getComponent<TransformComponent>()->getPositionRect();

	destRect = currentPositionRect;
	destRect.x -= (currentPositionRect.w / 2);
	destRect.y -= (currentPositionRect.h / 2);

	destRect.w += m_xRenderAdjustment;
	destRect.h += m_yRenderAdjustment;

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
SDL_Rect* RenderComponent::getRenderTextureRect(std::shared_ptr<GameObject>gameObject)
{

	SDL_Rect* textureSrcRect=nullptr;

	if (gameObject->getComponent<AnimationComponent>())
	{
		textureSrcRect = gameObject->getComponent<AnimationComponent>()->getCurrentAnimationTextureRect();
	}

	return textureSrcRect;

}

/*
Get the actual texture to display. If this is an animated object then it will have
different textures for different animation states
*/
SDL_Texture* RenderComponent::getRenderTexture(std::shared_ptr<GameObject>gameObject)
{

	SDL_Texture* texture = nullptr;

	if (gameObject->getComponent<AnimationComponent>())
	{
		texture = gameObject->getComponent<AnimationComponent>()->getCurrentAnimationTexture();
	}
	else 
	{
		texture = m_texture->sdlTexture;
	}

	return texture;
}


SDL_Surface* RenderComponent::getRenderSurface()
{

	return m_texture->surface;

}

void RenderComponent::outlineObject(std::shared_ptr<GameObject>gameObject, float lineSize)
{

	std::vector<SDL_FPoint> points;
	SDL_FRect gameObjectDrawRect = getRenderDestRect(gameObject);
	float saveScaleX, saveScaleY;
	SDL_FPoint point;

	//Adjust for camera
	if (gameObject->getComponent<TransformComponent>()->absolutePositioning() == false)
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

void RenderComponent::render(std::shared_ptr<GameObject>gameObject)
{
	//Get the various items for rendering
	SDL_Rect* textureSourceRect = getRenderTextureRect(gameObject);
	const SDL_FRect destRect = getRenderDestRect(gameObject);
	SDL_Texture* texture = getRenderTexture(gameObject);
	float angle = gameObject->getComponent<TransformComponent>()->angle();

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