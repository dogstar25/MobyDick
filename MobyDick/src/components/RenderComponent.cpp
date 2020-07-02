#include "RenderComponent.h"

RenderComponent::RenderComponent()
{

}

RenderComponent::~RenderComponent()
{

}

SDL_FRect RenderComponent::getRenderDestRect()
{
	SDL_FRect destRect;

	destRect = this->getPositionRect();

	destRect.w += this->definition()->xRenderAdjustment;
	destRect.h += this->definition()->yRenderAdjustment;

	//Adjust position based on current camera position - offset
	destRect.x -= Camera::instance().frame().x;
	destRect.y -= Camera::instance().frame().y;

	return destRect;
}

