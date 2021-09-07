#pragma once
#include <glad/glad.h>
#include <vector>
#include <memory>

#include "GLDrawer.h"
#include "Vertex.h"


class DrawBatch
{
public:

	DrawBatch() = default;
	DrawBatch(int ObjectType);

private:
	int ObjectType{}; //Sprite, Line, pixel, triangle
	GLuint shaderId{};
	GLuint textureId{};
	GLuint level{};

	std::vector<std::shared_ptr<Vertex>> m_vertexBatch{};
	std::shared_ptr<GLDrawer> m_glDrawer;

};

