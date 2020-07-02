#include "Renderer.h"

Renderer::Renderer()
{

}
Renderer::~Renderer()
{

}

Renderer& Renderer::instance()
{
	static Renderer singletonInstance;
	return singletonInstance;
}

void Renderer::render(SDL_Texture* texture, SDL_Color color, SDL_Rect* textureSourceRect, SDL_FRect* destRect, float angle)
{
	//Set the color
	SDL_SetTextureAlphaMod(texture, color.a);

	//SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);

	//Set the render color based on the objects color
	SDL_SetTextureColorMod(texture, color.r, color.g, color.b);

	//Render the texture
	SDL_RenderCopyExF(m_Renderer, texture, textureSourceRect, destRect, angle, NULL, SDL_FLIP_NONE);

}

void Renderer::render(SDL_FRect* destRect, SDL_Color color)
{

	//Render the rectangle
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRectF(m_Renderer, destRect);

}

