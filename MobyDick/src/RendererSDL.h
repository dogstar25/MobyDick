#ifndef RENDERER_SDL_H
#define RENDERER_SDL_H

#include <vector>
#include <map>
#include <optional>

#include <SDL2/SDL.h>
#include <box2d/box2d.h>

#include "Renderer.h"
#include "texture.h"


struct displayOverlay
{
	std::optional<SDL_Color> color;
	bool outlined;
	SDL_Color outlineColor;
	int outlineWidth;
};

namespace displayOverlays {

	static const displayOverlay outline_GREEN1{

		.color = std::nullopt,
		.outlined = true,
		.outlineColor = {0,255,0,255},
		.outlineWidth = {1}
	};

	/*
	....Add more displayOverlays here
	*/
}


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
	void drawLine(b2Vec2 lineStart, b2Vec2 lineEnd, SDL_Color color) override;
	void drawQuad(SDL_FRect quad, SDL_Color color, bool outline, SDL_Color outlineColor) override;
	void drawSprite(SDL_FRect quad, SDL_Color color, Texture* texture, SDL_Rect* textureSrcQuad, float angle, bool outline, SDL_Color outlineColor) override;

	void outlineObject(SDL_FRect quad, SDL_Color color);

	SDL_Renderer* sdlRenderer() {
		return m_sdlRenderer;
	}

private:
	SDL_Renderer* m_sdlRenderer;


};


#endif
