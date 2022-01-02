#pragma once
#include <glm/glm.hpp>

class Vertex
{

public:
	Vertex() = default;


};

class SpriteVertex : public Vertex
{

public:
	SpriteVertex() = default;

	glm::vec3 positionAttribute;
	glm::vec4 colorAttribute;
	glm::vec2 textureCoordsAttribute;


};

class LineVertex : public Vertex
{

public:
	LineVertex() = default;

	glm::vec3 positionAttribute;
	glm::vec4 colorAttribute;


};
