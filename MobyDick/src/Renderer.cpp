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

