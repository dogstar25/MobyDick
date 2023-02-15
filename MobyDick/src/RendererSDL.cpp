#include "RendererSDL.h"


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
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

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

//void RendererSDL::drawLine(b2Vec2 start, b2Vec2 end, SDL_Color color)
//{
//
//	SDL_SetRenderDrawColor(m_sdlRenderer, color.r, color.b, color.g, color.a);
//	SDL_RenderDrawLineF(m_sdlRenderer, start.x, start.y, end.x, end.y);
//
//}


void RendererSDL::drawSprite(SDL_FRect destQuad, SDL_Color color, Texture* texture, SDL_Rect* textureSrcQuad, float angle, 
	bool outline, SDL_Color outlineColor, RenderBlendMode textureBlendMode)
{

	if (textureBlendMode == RenderBlendMode::BLEND) {
		SDL_SetTextureBlendMode(texture->sdlTexture, SDL_BLENDMODE_BLEND);
	}
	else if (textureBlendMode == RenderBlendMode::ADD) {
		SDL_SetTextureBlendMode(texture->sdlTexture, SDL_BLENDMODE_ADD);
	}
	else{
		SDL_SetTextureBlendMode(texture->sdlTexture, SDL_BLENDMODE_NONE);
	}

	SDL_SetTextureAlphaMod(texture->sdlTexture, color.a);
	SDL_SetTextureColorMod(texture->sdlTexture, color.r, color.g, color.b);

	SDL_RendererFlip flip = SDL_RendererFlip::SDL_FLIP_NONE;
	if (texture->applyFlip == true) {
		flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;

	}

	//Render the texture
	SDL_RenderCopyExF(
		m_sdlRenderer,
		texture->sdlTexture,
		textureSrcQuad,
		&destQuad,
		angle,
		NULL,
		flip);
	
	//outline the object if defined to
	if (outline) {

		outlineObject(destQuad, outlineColor);
	}

}

void RendererSDL::renderPrimitives(int layerIndex)
{

	for (auto& line : m_primitiveLines) {

		SDL_SetRenderDrawColor(m_sdlRenderer, line.color.r, line.color.g, line.color.b, line.color.a);
		SDL_RenderDrawLineF(m_sdlRenderer, line.pointA.x, line.pointA.y, line.pointB.x, line.pointB.y);

	}

	m_primitiveLines.clear();

}

