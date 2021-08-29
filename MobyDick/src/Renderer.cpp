#include "Renderer.h"

#include "Globals.h"
#include "BaseConstants.h"
#include "GameConfig.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

Renderer::Renderer()
{

}
Renderer::~Renderer()
{

}

Renderer& Renderer::instance()
{
	static Renderer singletonInstance;
	return singletonInstance;
}

void Renderer::init(SDL_Window* window)
{
	//SDL_SetHint(SDL_HINT_RENDER_DRIVER, "openGL");
	SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");
	m_SDLRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(m_SDLRenderer, 0, 0, 0, 0);

}

bool Renderer::present()
{
	SDL_RenderPresent(m_SDLRenderer);
	SDL_SetRenderDrawColor(m_SDLRenderer, 0, 0, 0, 255);

	return true;
}

bool Renderer::clear()
{
	SDL_RenderClear(m_SDLRenderer);

	return true;
}

SDL_Texture* Renderer::createTextureFromSurface(SDL_Surface* surface)
{
	SDL_Texture* sdlTexture = SDL_CreateTextureFromSurface(m_SDLRenderer, surface);
	return sdlTexture;
}

//void Renderer::drawPoly(b2Body* body)
//{
//
//
//	b2Fixture* fixture = body->GetFixtureList();
//	while (fixture != NULL)
//	{
//
//		b2Shape* s = fixture->GetShape();
//		b2PolygonShape* shape = (b2PolygonShape*)s;
//
//		SDL_FPoint* points = new SDL_FPoint[shape->m_count + 1];
//
//		b2Vec2 firstVector;
//		bool firstFound = false;
//		SDL_FPoint point;
//		// Build list of transformed vertices
//		for (int i = 0; i < shape->m_count; ++i) {
//
//			b2Vec2 vector = shape->m_vertices[i];
//			//If this is the first vector, then save it
//			if (firstFound == false) {
//				firstVector = vector;
//				firstFound = true;
//			}
//
//			point.x = vector.x * GameConfig::instance().scaleFactor();
//			point.y = vector.y * GameConfig::instance().scaleFactor();
//			points[i] = point;
//		}
//
//		//Add the first point to the end to complete closed shape
//		point.x = firstVector.x;
//		point.y = firstVector.y;
//		points[shape->m_count] = point;
//
//		SDL_RenderDrawLinesF(m_SDLRenderer, points, shape->m_count + 1);
//
//		delete[] points;
//
//		fixture = body->GetFixtureList()->GetNext();
//	}
//
//}

void Renderer::drawPoints(SDL_FPoint* points, SDL_Color color)
{
	SDL_Color saveCurrentColor = {};
	SDL_GetRenderDrawColor(m_SDLRenderer, &saveCurrentColor.r, &saveCurrentColor.g, &saveCurrentColor.b, &saveCurrentColor.a);
	SDL_SetRenderDrawColor(m_SDLRenderer, color.r, color.b, color.g, color.a);

	SDL_RenderDrawLinesF(m_SDLRenderer, points, 5);

	SDL_SetRenderDrawColor(m_SDLRenderer, saveCurrentColor.r, saveCurrentColor.b, saveCurrentColor.g, saveCurrentColor.a);

}

void Renderer::drawLine(b2Vec2 start, b2Vec2 end, SDL_Color color)
{
	SDL_SetRenderDrawColor(m_SDLRenderer, color.r, color.b, color.g, color.a);
	SDL_RenderDrawLineF(m_SDLRenderer, start.x, start.y, end.x, end.y);

}

void Renderer::outlineObject(std::vector<SDL_FPoint> points, float lineSize, SDL_Color color)
{

	float saveScaleX, saveScaleY;

	//Set render scale to match linesize passed in
	SDL_RenderGetScale(m_SDLRenderer, &saveScaleX, &saveScaleY);
	SDL_RenderSetScale(m_SDLRenderer, lineSize, lineSize);

	this->drawPoints(points.data(), color);

	//Rest Scale to whatever is was before
	SDL_RenderSetScale(m_SDLRenderer, saveScaleX, saveScaleY);

}
