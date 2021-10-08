#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <map>
#include <optional>

#include <glm/glm.hpp>

#include "texture.h"

#include <SDL2/SDL.h>
#include <box2d/box2d.h>


//struct displayOverlay
//{
//	std::optional<SDL_Color> color;
//	bool outlined;
//	SDL_Color outlineColor;
//	int outlineWidth;
//};
//
//namespace displayOverlays {
//
//	static const displayOverlay outline_GREEN1{
//
//		.color = std::nullopt,
//		.outlined = true,
//		.outlineColor = {0,255,0,255},
//		.outlineWidth = {1}
//	};
//
//	/*
//	....Add more displayOverlays here
//	*/
//}


class Renderer
{
public:

	Renderer() = default;

	virtual void init(SDL_Window* window) = 0;;
	virtual bool present() = 0;
	virtual bool clear() = 0;
	virtual void drawLine(b2Vec2 lineStart, b2Vec2 lineEnd, SDL_Color color) = 0;
	virtual void drawQuad(SDL_FRect quad, SDL_Color color, bool outlines, SDL_Color outlineColor) = 0;
	virtual void drawSprite(SDL_FRect quad, SDL_Color color, Texture* texture, 
		SDL_Rect* textureSrcQuad, float angle, bool outline, SDL_Color outlineColor) = 0;

	//SDL Specific stuff
	virtual SDL_Texture* createTextureFromSurface(SDL_Surface* surface) = 0;
	virtual SDL_Renderer* sdlRenderer() = 0;

private:
	


};


#endif
