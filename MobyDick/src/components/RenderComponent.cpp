#include "RenderComponent.h"


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

	if (itrRender.isMember("textureBlendMode")) {
		m_textureBlendMode = static_cast<SDL_BlendMode>(EnumMap::instance().toEnum(itrRender["textureBlendMode"].asString()));
	}
	else {
		m_textureBlendMode = SDL_BLENDMODE_BLEND;
	}
	

	if (itrRender.isMember("outline")) {
		m_renderOutline = true;
		m_outlineThickness = itrRender["outline"]["thickness"].asInt();
		m_outLineColor =  util::JsonToColor(itrRender["outline"]["color"]);
	}
	else {
		m_renderOutline = false;
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
	SDL_FRect destRect;

	const auto& transformComponent = parent()->transformComponent.value();

	//Get its current position. Should be center of object
	destRect = transformComponent->getPositionRect();

	destRect.w += m_xRenderAdjustment;
	destRect.h += m_yRenderAdjustment;

	//Adjust position based on current camera position - offset
	if (transformComponent->absolutePositioning() == false)
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

	if (parent()->animationComponent) {
		textureSrcRect = parent()->animationComponent.value()->getCurrentAnimationTextureRect();
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

	if (parent()->animationComponent)
	{
		texture = parent()->animationComponent.value()->getCurrentAnimationTexture();
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

void RenderComponent::outlineObject(SDL_Color color)
{

	SDL_FRect gameObjectDrawRect = getRenderDestRect();

	SDL_SetRenderDrawColor(Renderer::instance().SDLRenderer(), color.r, color.g, color.b, 255);
	SDL_RenderDrawRectF(Renderer::instance().SDLRenderer(), &gameObjectDrawRect);

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
	const auto& transformComponent = parent()->transformComponent.value();

	const SDL_FRect positionRect = transformComponent->getPositionRect();
	SDL_Rect gameObjectPosRect={ (int)positionRect.x, (int)positionRect.y, (int)positionRect.w, (int)positionRect.h };
	SDL_Rect cameraRect = { (int)Camera::instance().frame().x,
		(int)Camera::instance().frame().y,
		(int)Camera::instance().frame().w+ Game::instance().worldTileWidth(),
		(int)Camera::instance().frame().h+ Game::instance().worldTileHeight() };

	/*
	If this object is within the viewable are or if its absolute positioned then render it
	*/
	if (SDL_HasIntersection(&gameObjectPosRect, &cameraRect) || 
		transformComponent->absolutePositioning() == true) {

		const SDL_FRect destRect = getRenderDestRect();
		SDL_Rect* textureSourceRect = getRenderTextureRect();
		SDL_Texture* texture = getRenderTexture();
		float angle = transformComponent->angle();

		//Set the color. Use the displayOverlay values if there is one
		if (m_displayOverlay.has_value() && m_displayOverlay->color.has_value()) {

			SDL_SetTextureAlphaMod(texture, m_displayOverlay->color->a);
			SDL_SetTextureColorMod(texture,
				m_displayOverlay->color->r,
				m_displayOverlay->color->g,
				m_displayOverlay->color->b);
		}
		else
		{
			SDL_SetTextureAlphaMod(texture, m_color.a);
			SDL_SetTextureColorMod(texture, m_color.r, m_color.g, m_color.b);
		}
		
		SDL_SetTextureBlendMode(texture, m_textureBlendMode);

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
		if (m_displayOverlay.has_value() && m_displayOverlay->outlined == true) {

			outlineObject(m_displayOverlay->outlineColor);
		}
		else {

			if (m_renderOutline == true) {
				outlineObject(m_outLineColor);
			}
		}
	}

}

//void RenderComponent::setDependencyReferences(GameObject* gameObject)
//{
//
//	if (gameObject->animationComponent) {
//		const auto& animationComponent = gameObject->animationComponent.value();
//		m_animationComponent = animationComponent.get();
//	}
//
//	auto transformComponent = gameObject->transformComponent.value();
//	m_transformComponent = transformComponent.get();
//
//}

void RenderComponent::applyDisplayOverlay(displayOverlay displayOverlay)
{
	m_displayOverlay = displayOverlay;
}

void RenderComponent::removeDisplayOverlay()
{
	m_displayOverlay.reset();
}

//void RenderComponent::render(SDL_FRect* destRect, SDL_Color color)
//{
//
//	//Render the rectangle
//	SDL_SetRenderDrawColor(Renderer::instance().SDLRenderer(), color.r, color.g, color.b, color.a);
//	SDL_RenderFillRectF(Renderer::instance().SDLRenderer(), destRect);
//
//}