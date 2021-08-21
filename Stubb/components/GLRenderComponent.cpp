#include "GLRenderComponent.h"


#include "../EnumMaps.h"
#include "../Camera.h"
#include "../game.h"

extern std::unique_ptr<Game> game;

GLRenderComponent::GLRenderComponent()
{

}


GLRenderComponent::GLRenderComponent(Json::Value definitionJSON)
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


GLRenderComponent::~GLRenderComponent()
{

}


void GLRenderComponent::update()
{

}

/*
Get the destination for rendering the gameObject
The end result should be a rectangle with a width and height in pixels and
an x, y position that is the top left corner of the object (for SDL render function)
*/
SDL_FRect GLRenderComponent::getRenderDestRect()
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
SDL_Rect* GLRenderComponent::getRenderTextureRect()
{

	SDL_Rect* textureSrcRect=nullptr;
	const auto& animationComponent = parent()->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);

	if (animationComponent)
	{
		textureSrcRect = animationComponent->getCurrentAnimationTextureRect();
	}

	return textureSrcRect;

}

/*
Get the actual texture to display. If this is an animated object then it will have
different textures for different animation states
*/
SDL_Texture* GLRenderComponent::getRenderTexture()
{

	SDL_Texture* texture = nullptr;
	const auto& animationComponent = parent()->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);


	if (animationComponent)
	{
		texture = animationComponent->getCurrentAnimationTexture();
	}
	else 
	{
		texture = m_texture->sdlTexture;
	}

	return texture;
}


SDL_Surface* GLRenderComponent::getRenderSurface()
{

	return m_texture->surface;

}

void GLRenderComponent::outlineObject(SDL_Color color)
{

	SDL_FRect gameObjectDrawRect = getRenderDestRect();

	SDL_SetRenderDrawColor(Renderer::instance().SDLRenderer(), color.r, color.g, color.b, 255);
	SDL_RenderDrawRectF(Renderer::instance().SDLRenderer(), &gameObjectDrawRect);

}

void GLRenderComponent::setColor(int red, int green, int blue, int alpha)
{
	m_color.r = red;
	m_color.g = green;
	m_color.b = blue;
	m_color.a = alpha;

}

void GLRenderComponent::render()
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

		SDL_SetRenderDrawColor(Renderer::instance().SDLRenderer(), m_color.r, m_color.g, m_color.b, m_color.a);
		SDL_RenderDrawLine(Renderer::instance().SDLRenderer(), static_cast<int>(start.x), static_cast<int>(start.y), static_cast<int>(end.x), static_cast<int>(end.y));

	}
	else if (parent()->m_gameObjectType == GameObjectType::RECTANGLE) {

		SDL_FRect retangle = { getRenderDestRect() };

		SDL_SetRenderDrawColor(Renderer::instance().SDLRenderer(), m_color.r, m_color.g, m_color.b, m_color.a);
		SDL_RenderFillRectF(Renderer::instance().SDLRenderer(), &retangle);

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
		If this object is within the viewable are or if its absolute positioned then render it
		*/
		if (SDL_HasIntersection(&gameObjectPosRect, &cameraRect) ||
			transform->absolutePositioning() == true) {

			const SDL_FRect destRect = getRenderDestRect();
			SDL_Rect* textureSourceRect = getRenderTextureRect();
			SDL_Texture* texture = getRenderTexture();
			float angle = transform->angle();

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

			//Drawa a red circle on the bodies center
			if(parent()->parentScene()->physicsConfig().b2DebugDrawMode == true &&
			   parent()->hasComponent(ComponentTypes::PHYSICS_COMPONENT)) {
				auto center = physics->physicsBody()->GetWorldCenter();
				DebugDraw::instance().DrawCircle(center, .1, b2Color(255, 0, 0, 255));
			}
		}
	}

}

void GLRenderComponent::setDependencyReferences(GameObject* gameObject)
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

void GLRenderComponent::applyDisplayOverlay(displayOverlay displayOverlay)
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