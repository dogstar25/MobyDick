#pragma once
#include <glad/glad.h>
#include <vector>
#include <memory>

#include "Vertex.h"


class DrawBatch
{
public:
	int vertextType; //Sprite, Line, pixel, triangle
	GLuint shaderId;
	GLuint textureId;
	GLuint level;
	GLuint vao;
	GLuint vbo;
	GLuint ibo;

	std::vector<std::shared_ptr<Vertex>> vertexBatch;

private:

};

