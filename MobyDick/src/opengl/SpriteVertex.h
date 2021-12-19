#pragma once
#include "Vertex.h"

#include <glm/glm.hpp>

class SpriteVertex : public Vertex
{
	
public:
	SpriteVertex() = default;

	glm::vec3 positionAttribute;
	glm::vec4 colorAttribute;
	glm::vec2 textureCoordsAttribute;
	float textureIndex;


};

