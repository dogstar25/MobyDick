#include "RenderSystem.h"

#include <SDL2/SDL.h>

#include "../game.h"
#include "../Camera.h"
#include "../components/TransformComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/AnimationComponent.h"
#include "../components/RenderComponent.h"
#include "../ecs/GameCoordinator.h"
#include "../Renderer.h"

void RenderSystem::update()
{
		for (auto& entity : mEntities)
		{
			auto& renderComponent = Game::instance().gameCoordinator().GetComponent<RenderComponent>(entity);
			auto& transformComponent = Game::instance().gameCoordinator().GetComponent<TransformComponent>(entity);

			//Check if this object is in the viewable area of the world
			//Add a tiles width to the camera to buffer it some
			const SDL_FRect positionRect = transformComponent.getPositionRect();
			SDL_Rect gameObjectPosRect = { positionRect.x, positionRect.y, positionRect.w, positionRect.h };
			SDL_Rect cameraRect = { Camera::instance().frame().x,
				Camera::instance().frame().y,
				Camera::instance().frame().w + Game::instance().worldTileWidth(),
				Camera::instance().frame().h + Game::instance().worldTileHeight() };

			/*
			If this object is within the viewable are or if its absolute positioned then render it
			*/
			if (SDL_HasIntersection(&gameObjectPosRect, &cameraRect) ||
				transformComponent.m_absolutePositioning == true)
			{
				const SDL_FRect destRect = getRenderDestRect(renderComponent, transformComponent);
				SDL_Rect* textureSourceRect = getRenderTextureRect(renderComponent);
				SDL_Texture* texture = getRenderTexture(renderComponent);
				float angle = transformComponent.m_angle;

				//Set the color
				SDL_SetTextureAlphaMod(texture, renderComponent.m_color.a);

				//SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);

				//Set the render color based on the objects color
				SDL_SetTextureColorMod(texture, renderComponent.m_color.r, renderComponent.m_color.g, renderComponent.m_color.b);

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
			else
			{
				int todd = 1;
			}
		}
}

void RenderSystem::init()
{
	/*
	Build the component signature for this system
	*/
	Signature signature;
	signature.set(Game::instance().gameCoordinator().GetComponentType<RenderComponent>());
	signature.set(Game::instance().gameCoordinator().GetComponentType<TransformComponent>());

	Game::instance().gameCoordinator().SetSystemSignature<RenderSystem>(signature);
}

/*
Get the destination for rendering the gameObject
The end result should be a rectangle with a width and height in pixels and
an x, y position that is the top left corner of the object (for SDL render function)
*/
SDL_FRect RenderSystem::getRenderDestRect(RenderComponent& renderComponent, TransformComponent& transformComponent)
{
	SDL_FRect destRect, currentPositionRect;

	//Get its current position. Should be center of object
	currentPositionRect = transformComponent.getPositionRect();

	destRect = currentPositionRect;
	destRect.x -= (currentPositionRect.w / 2);
	destRect.y -= (currentPositionRect.h / 2);

	destRect.w += renderComponent.m_xRenderAdjustment;
	destRect.h += renderComponent.m_yRenderAdjustment;

	//Adjust position based on current camera position - offset
	if (transformComponent.m_absolutePositioning == false)
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
SDL_Rect* RenderSystem::getRenderTextureRect(RenderComponent& renderComponent)
{

	return renderComponent.m_currentTextureAnimationSrcRect.get();

}

/*
Get the actual texture to display. If this is an animated object then it will have
different textures for different animation states
*/
SDL_Texture* RenderSystem::getRenderTexture(RenderComponent& renderComponent)
{

	return renderComponent.m_texture->sdlTexture;
}


//void RenderSystem::outlineObject(float lineSize)
//{
//
//	std::vector<SDL_FPoint> points;
//	SDL_FRect gameObjectDrawRect = getRenderDestRect();
//	float saveScaleX, saveScaleY;
//	SDL_FPoint point;
//
//	//Adjust for camera
//	if (m_transformComponent->absolutePositioning() == false)
//	{
//		gameObjectDrawRect.x -= Camera::instance().frame().x;
//		gameObjectDrawRect.y -= Camera::instance().frame().y;
//	}
//
//	//topleft
//	point.x = gameObjectDrawRect.x / lineSize;
//	point.y = gameObjectDrawRect.y / lineSize;
//	points.push_back(point);
//
//	//topright
//	point.x = (gameObjectDrawRect.x + gameObjectDrawRect.w) / lineSize;
//	point.y = gameObjectDrawRect.y / lineSize;
//	points.push_back(point);
//
//	//bottomright
//	point.x = (gameObjectDrawRect.x + gameObjectDrawRect.w) / lineSize;
//	point.y = (gameObjectDrawRect.y + gameObjectDrawRect.h) / lineSize;
//	points.push_back(point);
//
//	//bottomleft
//	point.x = gameObjectDrawRect.x / lineSize;
//	point.y = (gameObjectDrawRect.y + gameObjectDrawRect.h) / lineSize;
//	points.push_back(point);
//
//	//add the topleft as last point to complete the shape
//	point.x = gameObjectDrawRect.x / lineSize;
//	point.y = gameObjectDrawRect.y / lineSize;
//	points.push_back(point);
//
//	Renderer::instance().outlineObject(points, lineSize);
//
//	points.clear();
//
//}
