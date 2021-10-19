#include "RenderComponent.h"


#include "../EnumMaps.h"
#include "../Camera.h"
#include "../game.h"

#include <glm/glm.hpp>

extern std::unique_ptr<Game> game;

RenderComponent::RenderComponent()
{

}


RenderComponent::RenderComponent(Json::Value definitionJSON, int layer)
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
	m_layer = layer;

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

	const auto& transform = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

	//Get its current position. Should be center of object
	//destRect = m_transformComponent->getPositionRect();
	destRect = transform->getPositionRect();

	destRect.w += m_xRenderAdjustment;
	destRect.h += m_yRenderAdjustment;

	//Adjust position based on current camera position - offset
	if (transform->absolutePositioning() == false)
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
SDL_Rect* RenderComponent::getRenderTextureRect(Texture& texture)
{

	SDL_Rect* textureSrcRect=nullptr;
	const auto& animationComponent = parent()->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);

	if (animationComponent)
	{
		textureSrcRect = animationComponent->getCurrentAnimationTextureRect();
	}
	else {
		textureSrcRect = &texture.textureAtlasQuad;
	}

	return textureSrcRect;

}

/*
Get the actual texture to display. If this is an animated object then it will have
different textures for different animation states
*/
std::shared_ptr<Texture> RenderComponent::getRenderTexture()
{

	//SDL_Texture* texture = nullptr;
	std::shared_ptr<Texture> texture{};
	const auto& animationComponent = parent()->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);

	if (animationComponent)
	{
		texture = animationComponent->getCurrentAnimationTexture();
	}
	else 
	{
		texture = m_texture;
	}

	return texture;
}


SDL_Surface* RenderComponent::getRenderSurface()
{

	return m_texture->surface;

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

	const auto& transform = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	const auto& physics = parent()->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);

	if (parent()->m_gameObjectType == GameObjectType::LINE) {

		b2Vec2 start = transform->lineStart();
		b2Vec2 end = transform->lineEnd();

		if (transform->absolutePositioning() == false)
		{
			start.x -= Camera::instance().frame().x;
			start.y -= Camera::instance().frame().y;
			end.x -= Camera::instance().frame().x;
			end.y -= Camera::instance().frame().y;

		}

		game->renderer()->drawLine(transform->lineStart(), transform->lineEnd(), m_color);

	}
	else if (parent()->m_gameObjectType == GameObjectType::RECTANGLE) {

		SDL_FRect quad = { getRenderDestRect() };
		bool outline{};
		SDL_Color outlineColor{};

		//Outline the gameObject if defined to . Either from a displayoverlay or by the objects definition
		if (m_displayOverlay.has_value() && m_displayOverlay->outlined == true) {

			outline = true;
			outlineColor = m_displayOverlay->outlineColor;
		}
		else {

			outline = true;
			outlineColor = m_outLineColor;
		}

		game->renderer()->drawQuad(quad, m_color, outline, outlineColor);

	}
	else if (parent()->m_gameObjectType == GameObjectType::SPRITE) {
		
		//Check if this object is in the viewable area of the world
		//Add a tiles width to the camera to buffer it some
		const SDL_FRect positionRect = transform->getPositionRect();
		SDL_Rect gameObjectPosRect = { (int)positionRect.x, (int)positionRect.y, (int)positionRect.w, (int)positionRect.h };
		SDL_Rect cameraRect = { (int)Camera::instance().frame().x,
			(int)Camera::instance().frame().y,
			(int)Camera::instance().frame().w + game->worldTileWidth(),
			(int)Camera::instance().frame().h + game->worldTileHeight() };

		/*
		If this object is within the viewable are or if its absolute positioned and therefore is not dependent on the camera
		then render it
		*/
		if (SDL_HasIntersection(&gameObjectPosRect, &cameraRect) ||
			transform->absolutePositioning() == true) {

			bool outline{};
			SDL_Color outlineColor{};
			float angle = transform->angle();

			Texture* texture = getRenderTexture().get();

			SDL_FRect destQuad = getRenderDestRect();
			SDL_Rect* textureSourceQuad = getRenderTextureRect(*texture);

			//SDL Only Stuff
			if (GameConfig::instance().rendererType() == RendererType::SDL) {
				SDL_Texture* sdlTexture = getRenderTexture()->sdlTexture;
				SDL_SetTextureBlendMode(sdlTexture, m_textureBlendMode);
			}
			/////

			//Outline the gameObject if defined to 
			if (m_displayOverlay.has_value() && m_displayOverlay->outlined == true) {

				outline = true;
				outlineColor = m_displayOverlay->outlineColor;
			}
			else {

				outline = true;
				outlineColor = m_outLineColor;

			}

			game->renderer()->drawSprite(destQuad, m_color, m_layer, texture, textureSourceQuad, angle, outline, outlineColor);

			//Drawa a red circle on the bodies center
			//if(parent()->parentScene()->physicsConfig().b2DebugDrawMode == true &&
			//   parent()->hasComponent(ComponentTypes::PHYSICS_COMPONENT)) {
			//	auto center = physics->physicsBody()->GetWorldCenter();
			//	DebugDraw::instance().DrawCircle(center, .1, b2Color(255, 0, 0, 255));
			//}
		}
	}

}

void RenderComponent::setDependencyReferences(GameObject* gameObject)
{
	//auto& animationComponent = parent()->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);

	//if (gameObject->hasComponent(ComponentTypes::ANIMATION_COMPONENT)) {
	//	auto animationComponent = gameObject->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);
	//	m_animationComponent = animationComponent.get();
	//}

	//if (gameObject->hasComponent(ComponentTypes::TRANSFORM_COMPONENT)) {
	//	auto transformComponent = gameObject->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	//	m_transformComponent = transformComponent.get();
	//}

}

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
//	SDL_SetRenderDrawColor(RendererSDL::instance().renderer(), color.r, color.g, color.b, color.a);
//	SDL_RenderFillRectF(RendererSDL::instance().renderer(), destRect);
//
//}