#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <map>
#include <optional>

#include <SDL2/SDL.h>
#include <box2d/box2d.h>


struct DisplayScheme
{
	std::optional<SDL_Color> color;
	bool outlined;
	SDL_Color outlineColor;
	int outlineWidth;
};

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
	void drawPoints(SDL_FPoint*, SDL_Color color);
	void drawLine(b2Vec2, b2Vec2, SDL_Color color);
	void outlineObject(std::vector<SDL_FPoint> points, float lineSize, SDL_Color color);

	SDL_Renderer* SDLRenderer() {
		return m_SDLRenderer;
	}

	DisplayScheme getDisplayScheme(uint8_t displayScheme) {

		return m_displaySchemes.at(displayScheme);

	}

private:
	SDL_Renderer* m_SDLRenderer;
	std::map<int, DisplayScheme> m_displaySchemes;

	void _loadDisplaySchemes();


};


#endif
