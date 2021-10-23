#include "RendererSDL.h"

#include "Util.h"
#include "BaseConstants.h"
#include "GameConfig.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

RendererSDL::RendererSDL()
{

}
RendererSDL::~RendererSDL()
{

}

void RendererSDL::init(SDL_Window* window)
{
	//SDL_SetHint(SDL_HINT_RENDER_DRIVER, "openGL");
	SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");
	m_sdlRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(m_sdlRenderer, 0, 0, 0, 0);

}

bool RendererSDL::present()
{
	SDL_RenderPresent(m_sdlRenderer);
	SDL_SetRenderDrawColor(m_sdlRenderer, 0, 0, 0, 255);

	return true;
}

bool RendererSDL::clear()
{
	SDL_RenderClear(m_sdlRenderer);

	return true;
}

SDL_Texture* RendererSDL::createTextureFromSurface(SDL_Surface* surface)
{
	SDL_Texture* sdlTexture = SDL_CreateTextureFromSurface(m_sdlRenderer, surface);
	return sdlTexture;
}


void RendererSDL::drawPoints(SDL_FPoint* points, SDL_Color color)
{
	SDL_Color saveCurrentColor = {};
	SDL_GetRenderDrawColor(m_sdlRenderer, &saveCurrentColor.r, &saveCurrentColor.g, &saveCurrentColor.b, &saveCurrentColor.a);
	SDL_SetRenderDrawColor(m_sdlRenderer, color.r, color.b, color.g, color.a);

	SDL_RenderDrawLinesF(m_sdlRenderer, points, 5);

	SDL_SetRenderDrawColor(m_sdlRenderer, saveCurrentColor.r, saveCurrentColor.b, saveCurrentColor.g, saveCurrentColor.a);

}

void RendererSDL::drawLine(b2Vec2 start, b2Vec2 end, SDL_Color color)
{

	SDL_SetRenderDrawColor(m_sdlRenderer, color.r, color.b, color.g, color.a);
	SDL_RenderDrawLineF(m_sdlRenderer, start.x, start.y, end.x, end.y);

}

void RendererSDL::drawQuad(SDL_FRect quad, SDL_Color color, bool outline, SDL_Color outlineColor)
{
	SDL_SetRenderDrawColor(m_sdlRenderer, color.r, color.g, color.b, color.a);
	SDL_SetRenderDrawBlendMode(m_sdlRenderer, SDL_BLENDMODE_BLEND);
	SDL_RenderFillRectF(m_sdlRenderer, &quad);

	if (outline) {

		outlineObject(quad, outlineColor);
	}
}

void RendererSDL::drawSprite(SDL_FRect destQuad, SDL_Color color, int layer, Texture* texture, SDL_Rect* textureSrcQuad, float angle, bool outline, SDL_Color outlineColor)
{

	SDL_SetTextureAlphaMod(texture->sdlTexture, color.a);
	SDL_SetTextureColorMod(texture->sdlTexture, color.r, color.g, color.b);

	//Render the texture
	SDL_RenderCopyExF(
		m_sdlRenderer,
		texture->sdlTexture,
		textureSrcQuad,
		&destQuad,
		angle,
		NULL,
		SDL_FLIP_NONE);

}

void RendererSDL::outlineObject(SDL_FRect quad, SDL_Color color)
{

	SDL_SetRenderDrawColor(m_sdlRenderer, color.r, color.g, color.b, 255);
	SDL_RenderDrawRectF(m_sdlRenderer, &quad);

}

