#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <map>
#include <optional>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <box2d/box2d.h>
#include "imgui.h"


enum class RenderBlendMode {

	BLEND,
	ADD,
	NONE

};

struct PrimitivePoint{
	glm::vec2 point{};
	glm::uvec4 color{};
};

class PrimitiveLine {

public:
	//PrimitiveLine(glm::vec2 pointA, glm::vec2 pointB, glm::vec4 color) :
	//	pointA(pointA), pointB(pointB), color(color) {}
	glm::vec2 pointA{};
	glm::vec2 pointB{};
	glm::uvec4 color{};
};


class Texture;

class Renderer
{
public:

	Renderer() = default;

	virtual void init(SDL_Window* window) = 0;;
	virtual bool present() = 0;
	virtual bool clear() = 0;
	virtual void drawSprite(SDL_FRect quad, SDL_Color color, Texture* texture, SDL_Rect* textureSrcQuad, float angle, 
		bool outline, SDL_Color outlineColor, RenderBlendMode textureBlendMode) = 0;
	virtual void renderPrimitives(int layerIndex) = 0;

	void addLine(glm::vec2 pointA, glm::vec2 pointB, glm::uvec4 color);
	void addLine(PrimitiveLine line);
	void drawPoints(std::vector<SDL_FPoint> points, SDL_Color color = { 255,255,255,255 });
	//void initImGuiFonts(ImGuiIO& io);
	

	//SDL Specific stuff
	virtual SDL_Texture* createTextureFromSurface(SDL_Surface* surface) = 0;
	virtual SDL_Renderer* sdlRenderer() = 0;

	////ImGuiFonts
	//ImFont* font12() { return m_font12; }
	//ImFont* font22() { return m_font22; }
	//ImFont* font32() { return m_font32; }
	//ImFont* font64() { return m_font64; }

protected:

	void outlineObject(SDL_FRect quad, SDL_Color color);
	
	//ToDo:Add a layer level - all outlines will be put in a arbitrary layer below the menu/gui layer
	std::vector<PrimitivePoint> m_primitivePoints{};
	std::vector<PrimitiveLine> m_primitiveLines{};

	//ImFont* m_font12{};
	//ImFont* m_font22{};
	//ImFont* m_font32{};
	//ImFont* m_font64{};




};


#endif
