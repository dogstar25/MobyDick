#include "Renderer.h"

void Renderer::addLine(glm::vec2 pointA, glm::vec2 pointB, glm::uvec4 color)
{

	//PrimitiveLine line = PrimitiveLine( pointA, pointB, color );
	PrimitiveLine line{ pointA, pointB, color };
	m_primitiveLines.push_back(line);

}
void Renderer::addLine(PrimitiveLine line)
{



}

void Renderer::outlineObject(SDL_FRect quad, SDL_Color color)
{

	//SDL_SetRenderDrawColor(m_sdlRenderer, color.r, color.g, color.b, 255);
	//SDL_RenderDrawRectF(m_sdlRenderer, &quad);

	glm::vec2 topLeftPoint(quad.x, quad.y);
	glm::vec2 topRightPoint(quad.x + quad.w, quad.y);
	glm::vec2 bottomRightPoint(quad.x + quad.w, quad.y + quad.h);
	glm::vec2 bottomLeftPoint(quad.x, quad.y + quad.h);

	glm::uvec4 lineColor(color.r, color.g, color.b, color.a);

	//top line
	addLine(topLeftPoint, topRightPoint, lineColor);

	//right side line
	addLine(topRightPoint, bottomRightPoint, lineColor);

	//bottom line
	addLine(bottomRightPoint, bottomLeftPoint, lineColor);

	//Left line
	addLine(bottomLeftPoint, topLeftPoint, lineColor);

}

void Renderer::drawPoints(std::vector<SDL_FPoint> points, SDL_Color color)
{
	glm::vec2 pointB{};

	for (int i = 0; i < points.size()-1;i++) {

		glm::vec2 pointA = { points[i].x, points[i].y };

		//If
		glm::vec2 pointB = { points[i+1].x, points[i+1].y };

		addLine(pointA, pointB, {color.r, color.g, color.b, color.a});

	}


}

