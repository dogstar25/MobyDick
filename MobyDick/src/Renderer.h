#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <map>
#include <optional>

#include <glm/glm.hpp>

//#include "texture.h"

#include <SDL2/SDL.h>
#include <box2d/box2d.h>

struct PrimitivePoint{
	glm::vec2 point{};
	glm::vec4 color{};
};

struct PrimitiveLine {

	glm::vec2 pointA{};
	glm::vec2 pointB{};
	glm::vec4 color{};
};


class Texture;

class Renderer
{
public:

	Renderer() = default;

	virtual void init(SDL_Window* window) = 0;;
	virtual bool present() = 0;
	virtual bool clear() = 0;
	virtual void draw(SDL_FRect quad, SDL_Color color, int layer, Texture* texture, SDL_Rect* textureSrcQuad, float angle, bool outline, SDL_Color outlineColor) = 0;
	virtual void renderPrimitives(int layerIndex) = 0;

	//SDL Specific stuff
	virtual SDL_Texture* createTextureFromSurface(SDL_Surface* surface) = 0;
	virtual SDL_Renderer* sdlRenderer() = 0;

private:
	
	std::vector<PrimitivePoint> m_primitivePoints{};
	std::vector<PrimitiveLine> m_primitiveLines{};



};


#endif
