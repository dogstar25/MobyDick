#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

struct SpriteindexBuffer {
	glm::uvec3 topLeftTriangle{ 0,1,2 };
	glm::uvec3 botRightTriangle{ 2,3,0 };
};


class IndexBuffer
{

private:

	//This will grow as we add stuff to a particular draw batch>?
	std::vector<glm::uint> m_indexes;

public:

};

