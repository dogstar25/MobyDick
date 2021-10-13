#include "IndexBuffer.h"

void IndexBuffer::addSpriteIndexBuffer()
{
	//Sprite Index format
	//0,1,2, top right triangle
	//2,3,0, bottom left triangle

	//Create the new set of 6 index items
	for (int i = 0; i < 6; i++) {
		m_indexes.push_back(spriteindexBuffer[i] + m_objectCount);
	}

	m_objectCount++;

}

void IndexBuffer::clear()
{
	m_indexes.clear();
	m_objectCount = 0;
}
