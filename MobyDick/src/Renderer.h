#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include <SDL2/SDL.h>
#include <box2d/box2d.h>

class Renderer
{
public:
	Renderer();
	~Renderer();

	void init(SDL_Window* window);
	static Renderer& instance();
	bool present();
	bool clear();
	SDL_Texture* createTextureFromSurface(SDL_Surface* surface);
	void drawPoly(b2Body* body);
	void drawPoints(SDL_FPoint*);
	void drawLine(b2Vec2, b2Vec2);
	void outlineObject(std::vector<SDL_FPoint> points, float lineSize);


	SDL_Renderer* SDLRenderer() {
		return m_SDLRenderer;
	}

private:
	SDL_Renderer* m_SDLRenderer;

};


#endif
