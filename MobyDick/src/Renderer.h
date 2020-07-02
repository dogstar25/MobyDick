#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>

class Renderer
{
public:
	Renderer();
	~Renderer();

	void render(SDL_Texture* texture,
		SDL_Color color,
		SDL_Rect* textureSourceRect,
		SDL_FRect* destRect,
		float angle);

	void render(SDL_FRect* destRect, SDL_Color color);
	static Renderer& instance();

	SDL_Renderer* SDLRenderer() {
		return m_SDLRenderer;
	}

private:
	SDL_Renderer* m_SDLRenderer;

};


#endif
