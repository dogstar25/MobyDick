#pragma once
#include "DrawBatch.h"
#include <memory>

#include "GLDrawer.h"
#include "Vertex.h"
#include "Shader.h"
#include "../texture.h"


class LineDrawBatch : public DrawBatch
{

public:

	LineDrawBatch() = default;
	LineDrawBatch( GLShaderType shaderType);

	void addVertexBuffer(const std::vector<LineVertex>& lineVertices) override;
	void draw() override;
	void clear() override;


private:

	std::vector<LineVertex> m_vertexBatch{};

};

