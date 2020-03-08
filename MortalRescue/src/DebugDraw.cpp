#include "DebugDraw.h"
#include "Game.h"
#include "TextureManager.h"
#include "GameConfig.h"

#include <SDL.h>

extern Game* game;

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{

	DrawSolidPolygon(vertices, vertexCount, color);

}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{

	SDL_Point *points = new SDL_Point[vertexCount + 1];

	SDL_Point firstPoint;
	bool firstFound = false;
	SDL_Point point;

	for (int i = 0; i < vertexCount; ++i) {

		b2Vec2 vector = vertices[i];

		//Adjust points based on Gamescale
		point.x = vector.x * GameConfig::instance().scaleFactor();
		point.y = vector.y * GameConfig::instance().scaleFactor();

		//Adjust position based on current camera position - offset
		point.x -= Camera::instance().frame().x;
		point.y -= Camera::instance().frame().y;

		//If this is the first point, then save it
		if (firstFound == false) {
			firstPoint = point;
			firstFound = true;
		}

		//Add point to point array
		points[i] = point;
	}

	//Add the first point to the end to complete closed shape
	points[vertexCount] = firstPoint;

	TextureManager::instance().drawPoints(points);

	delete[] points;
}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	b2Vec2 axis;
	DrawSolidCircle(center, radius, axis, color);
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{

	//convert to propert game scale
	b2Vec2 newCenter = center;
	newCenter.x *= GameConfig::instance().scaleFactor();
	newCenter.y *= GameConfig::instance().scaleFactor();
	radius *= GameConfig::instance().scaleFactor();

	//Adjust position based on current camera position - offset
	newCenter.x -= Camera::instance().frame().x;
	newCenter.y -= Camera::instance().frame().y;

	int  sides = (2 * M_PI) * radius / 2;

	float d_a = (2 * M_PI) / sides,
		angle = d_a;

	b2Vec2 start, end;
	end.x = radius;
	end.y = 0.0f;
	end = end + newCenter;
	for (int i = 0; i != sides; i++)
	{
		start = end;
		end.x = cos(angle) * radius;
		end.y = sin(angle) * radius;
		end = end + newCenter;
		angle += d_a;
		
		TextureManager::instance().drawLine(start, end);
	}


}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	bool a;
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
	bool a;
}

void DebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	bool a;
}

void DebugDraw::DrawString(int x, int y, const char *string, ...)
{
	bool a;
}

void DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& c)
{
	SDL_Rect poly;
	poly.x = aabb->upperBound.x;
	poly.y = aabb->upperBound.y;
	poly.w = aabb->GetExtents().x * 2;
	poly.h = aabb->GetExtents().y * 2;
	//SDL_BlitSurface(Screen::white, &poly, Screen::screen, &poly);
}