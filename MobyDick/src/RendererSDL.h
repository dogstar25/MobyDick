#ifndef RENDERER_SDL_H
#define RENDERER_SDL_H

#include <vector>
#include <map>
#include <optional>

#include <SDL2/SDL.h>
#include <box2d/box2d.h>

#include "Renderer.h"
#include "texture.h"

class RendererSDL : public Renderer
{
public:
	RendererSDL();
	~RendererSDL();

	void init(SDL_Window* window);

	bool present();
	bool clear();
	SDL_Texture* createTextureFromSurface(SDL_Surface* surface);
	void drawPoints(SDL_FPoint*, SDL_Color color);
	void drawSprite(SDL_FRect quad, SDL_Color color, Texture* texture, SDL_Rect* textureSrcQuad, float angle, 
		bool outline, SDL_Color outlineColor, RenderBlendMode textureBlendMode) override;

	void renderPrimitives(int layerIndex);

	SDL_Renderer* sdlRenderer() {
		return m_sdlRenderer;
	}

private:
	SDL_Renderer* m_sdlRenderer;


};


#endif
