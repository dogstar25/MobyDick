#include "RenderComponent.h"

#include "../Renderer.h"
#include "../GameObject.h"
#include "../Camera.h"
#include "../TextureManager.h"

/*
Used for GameObject/Component creation
*/
RenderComponent::RenderComponent(RenderComponent* componentDefinition)
{

}

/*
Used for GameObject/Component definition data
*/

RenderComponent::RenderComponent(Json::Value& componentDetailsJSON)
{
	//Convenience reference
	Json::Value itr = componentDetailsJSON;

	if (itr.isMember("color") )
	{
		setColor( 
			itr["color"]["red"].asInt(),
			itr["color"]["green"].asInt(),
			itr["color"]["blue"].asInt(),
			itr["color"]["alpha"].asInt()
		);
	}
	else
	{
		setColor(255,255,255,255);
	}

	m_textureId = itr["textureId"].asString();
	m_xRenderAdjustment = itr["xRenderAdjustment"].asFloat();
	m_yRenderAdjustment = itr["yRenderAdjustment"].asFloat();
	m_renderOutline = itr["renderOutline"].asFloat();

}

RenderComponent::~RenderComponent()
{

}

/*
Get the destination for rendering the gameObject
*/
SDL_FRect RenderComponent::getRenderDestRect()
{
	SDL_FRect destRect;

	destRect = m_transformComponent->getPositionRect();

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
SDL_Rect* RenderComponent::getRenderTextureRect()
{
	SDL_Rect* textureSrcRect=nullptr;

	//if (m_parentGameObject->gameObjectDefinition()->hasComponent(ANIMATION_COMPONENT)) 
	if(m_animationComponent)
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

	if (m_animationComponent) {

		texture = m_animationComponent->getCurrentAnimationTexture();
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
	if (m_transformComponent->absolutePositioning() == false)
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

	TextureManager::instance().outlineObject(points, lineSize);

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

}

//void RenderComponent::render(SDL_FRect* destRect, SDL_Color color)
//{
//
//	//Render the rectangle
//	SDL_SetRenderDrawColor(Renderer::instance().SDLRenderer(), color.r, color.g, color.b, color.a);
//	SDL_RenderFillRectF(Renderer::instance().SDLRenderer(), destRect);
//
//}