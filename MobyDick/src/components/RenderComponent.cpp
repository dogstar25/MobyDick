#include "RenderComponent.h"

#include <json/json.h>

#include "../Renderer.h"
#include "../Camera.h"
#include "../GameObjectManager.h"
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

	m_gameObjectId = definitionJSON["id"].asString();;

	if (itrRender.isMember("color"))
	{
		m_color = util::JsonToColor(itrRender["color"]);
	}
	else
	{
		setColor(255, 255, 255, 255);
	}

	m_textureId = itrRender["textureId"].asString();
	m_xRenderAdjustment = itrRender["xRenderAdjustment"].asFloat();
	m_yRenderAdjustment = itrRender["yRenderAdjustment"].asFloat();

	if (itrRender.isMember("outline")) {
		m_renderOutline = true;
		m_outlineThickness = itrRender["outline"]["thickness"].asInt();
		m_outLineColor =  util::JsonToColor(itrRender["outline"]["color"]);
	}

	//Get Texture
	m_texture = TextureManager::instance().getTexture(itrRender["textureId"].asString());

}


RenderComponent::~RenderComponent()
{

}


void RenderComponent::update()
{

}

/*
Get the destination for rendering the gameObject
The end result should be a rectangle with a width and height in pixels and
an x, y position that is the top left corner of the object (for SDL render function)
*/
SDL_FRect RenderComponent::getRenderDestRect()
{
	SDL_FRect destRect, currentPositionRect;

	//Get its current position. Should be center of object
	destRect = m_transformComponent->getPositionRect();

	destRect.w += m_xRenderAdjustment;
	destRect.h += m_yRenderAdjustment;

	//Adjust position based on current camera position - offset
	if (m_transformComponent->absolutePositioning() == false)
	{
		destRect.x -= Camera::instance().frame().x;
		destRect.y -= Camera::instance().frame().y;
	}

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

	if (m_animationComponent)
	{
		textureSrcRect = m_animationComponent->getCurrentAnimationTextureRect();
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

	if (m_animationComponent)
	{
		texture = m_animationComponent->getCurrentAnimationTexture();
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

void RenderComponent::outlineObject(float lineSize, SDL_Color color)
{

	std::vector<SDL_FPoint> points;
	SDL_FRect gameObjectDrawRect = getRenderDestRect();
	float saveScaleX, saveScaleY;
	SDL_FPoint point;

	////Adjust for camera
	/*if (m_transformComponent->absolutePositioning() == false)
	{
		gameObjectDrawRect.x -= Camera::instance().frame().x;
		gameObjectDrawRect.y -= Camera::instance().frame().y;
	}*/

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

	Renderer::instance().outlineObject(points, lineSize, color);

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
	//Check if this object is in the viewable area of the world
	//Add a tiles width to the camera to buffer it some
	const SDL_FRect positionRect = m_transformComponent->getPositionRect();
	SDL_Rect gameObjectPosRect = { positionRect.x, positionRect.y, positionRect.w, positionRect.h };
	SDL_Rect cameraRect = { Camera::instance().frame().x, 
		Camera::instance().frame().y, 
		Camera::instance().frame().w+ Game::instance().worldTileWidth(),
		Camera::instance().frame().h+ Game::instance().worldTileHeight() };
	
	/*
	If this object is within the viewable are or if its absolute positioned then render it
	*/
	if (SDL_HasIntersection(&gameObjectPosRect, &cameraRect) || 
		m_transformComponent->absolutePositioning() == true)
	{
		const SDL_FRect destRect = getRenderDestRect();
		SDL_Rect* textureSourceRect = getRenderTextureRect();
		SDL_Texture* texture = getRenderTexture();
		float angle = m_transformComponent->angle();

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

		//Outline the gameObject if defined to 
		if (m_renderOutline == true) {
			outlineObject(m_outlineThickness, m_outLineColor);
		}
	}

}

void RenderComponent::setDependencyReferences(GameObject* gameObject)
{

	m_animationComponent = gameObject->getComponent<AnimationComponent>().get();
	m_transformComponent = gameObject->getComponent<TransformComponent>().get();

}



//void RenderComponent::render(SDL_FRect* destRect, SDL_Color color)
//{
//
//	//Render the rectangle
//	SDL_SetRenderDrawColor(Renderer::instance().SDLRenderer(), color.r, color.g, color.b, color.a);
//	SDL_RenderFillRectF(Renderer::instance().SDLRenderer(), destRect);
//
//}