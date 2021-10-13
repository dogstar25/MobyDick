#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

//struct SpriteindexBuffer {
//	glm::uvec3 topRightTriangle{ 0,1,2 };
//	glm::uvec3 botLeftTriangle{ 2,3,0 };
//};

const int spriteindexBuffer[] = {0,1,2,2,3,0};

class IndexBuffer
{

private:

	//This will grow as we add stuff to a particular draw batch>?
	std::vector<glm::uint> m_indexes;
	int m_objectCount{0}; //each object is 6 integers

public:

	void addSpriteIndexBuffer();
	void clear();
	std::vector<glm::uint> const& indexes() { return  m_indexes; }
};

