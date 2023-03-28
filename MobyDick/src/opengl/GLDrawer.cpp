#include "GLDrawer.h"


#include "../game.h"

extern std::unique_ptr<Game> game;

GLDrawer::GLDrawer(GLDrawerType drawerType)
{

	//Generate and bind to a new VOA (Vertex Object Array)
	glGenVertexArrays(1, &m_vaoId);
	glBindVertexArray(m_vaoId);

	//Generate and bind to a VBO
	glGenBuffers(1, &m_vboId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);

	_setVertexBufferAttriubuteLayout(drawerType);

	//Generate a IBO
	glGenBuffers(1, &m_iboId);

	//unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	switch (drawerType){
		case GLDrawerType::GLSPRITE:
			m_vertexSize = sizeof(SpriteVertex);
			break;
		case GLDrawerType::GLLINE:
			//m_vertexSize = sizeof(SpriteVertex);
			break;
	}



}

GLDrawer::~GLDrawer()
{
}

void GLDrawer::prepare()
{

	glBindVertexArray(m_vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);


}


void GLDrawer::draw(const std::vector<SpriteVertex>& spriteVertices, const std::vector<glm::uint>& spriteVertexIndexes, 
	Shader& shader, Texture* texture, RenderBlendMode textureBlendMode)
{

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_BLEND);

	if (textureBlendMode == RenderBlendMode::BLEND) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else if (textureBlendMode == RenderBlendMode::ADD) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
	else if (textureBlendMode == RenderBlendMode::MULTIPLY) {
		glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
	}
	else if (textureBlendMode == RenderBlendMode::MODULATE) {
		glBlendFunc(GL_DST_COLOR, GL_ZERO);
	}
	else if (textureBlendMode == RenderBlendMode::NONE) {
		glBlendFunc(GL_ONE, GL_ZERO);
	}
	
	prepare();

	glBufferData(GL_ARRAY_BUFFER, sizeof(SpriteVertex) * spriteVertices.size(), nullptr, GL_DYNAMIC_DRAW);

	//Use the program first
	glUseProgram(shader.shaderProgramId());

	//Set the Projection matrix uniform
	GLuint matrixId = glGetUniformLocation(shader.shaderProgramId(), "u_projection_matrix");
	auto projection_matrix = static_cast<GLRenderer*>(game->renderer())->projectionMatrix();
	glUniformMatrix4fv(matrixId, 1, false, (float*)&projection_matrix);

	//Initialize the texture and set the texture uniform

	GLuint textureArrayUniformId = glGetUniformLocation(shader.shaderProgramId(), "u_Texture");
	glUniform1i(textureArrayUniformId, GL_TEXTURE0);

	//Texture Index
	// 0 = Texture Atlas
	// 1 = Any Dynamically Generated Texture

	if (texture != nullptr) {
		GLuint textureId = static_cast<GLRenderer*>(game->renderer())->getTextureId(texture->openglTextureIndex);
		glBindTexture(GL_TEXTURE_2D, textureId);

		if (texture->openglTextureIndex == GL_TextureIndexType::DYNAMICALLY_LOADED) {
			static_cast<GLRenderer*>(game->renderer())->prepTexture(texture);
		}

	}

	//Submit the vertices
	auto bufferSize = spriteVertices.size() * sizeof(SpriteVertex);
	glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, &spriteVertices[0]);

	//Submit the vertex indices
	auto indexBufferSize = sizeof(GL_UNSIGNED_INT) * spriteVertexIndexes.size();
	//auto indexBufferSize = sizeof(glm::uint) * m_indexes.size();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, &spriteVertexIndexes[0], GL_DYNAMIC_DRAW);

	glDrawElements(GL_TRIANGLES, (GLsizei)spriteVertexIndexes.size(), GL_UNSIGNED_INT, 0);

}

void GLDrawer::draw(const std::vector<LineVertex>& lineVertices, int vertexCount, Shader& shader)
{

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_BLEND);
	//GL_ONE_MINUS_SRC_ALPHA
	//GL_DST_ALPHA
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	prepare();

	glBufferData(GL_ARRAY_BUFFER, sizeof(LineVertex) * lineVertices.size(), nullptr, GL_DYNAMIC_DRAW);

	//Use the program first
	glUseProgram(shader.shaderProgramId());

	//Set the Projection matrix uniform
	GLuint matrixId = glGetUniformLocation(shader.shaderProgramId(), "u_projection_matrix");
	auto projection_matrix = static_cast<GLRenderer*>(game->renderer())->projectionMatrix();
	glUniformMatrix4fv(matrixId, 1, false, (float*)&projection_matrix);

	//Submit the vertices
	auto swize = sizeof(LineVertex);
	auto bufferSize = lineVertices.size() * sizeof(LineVertex);
	glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, &lineVertices[0]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), &lineVertices[0], GL_DYNAMIC_DRAW);

	//Submit the vertex indices
	//const std::vector<glm::uint>lineVertexIndexes{0,1};
	//auto indexBufferSize = sizeof(GL_UNSIGNED_INT) * lineVertexIndexes.size();
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, &lineVertexIndexes[0], GL_DYNAMIC_DRAW);

	glDrawArrays(GL_LINES, 0, vertexCount);
	//glDrawElements(GL_TRIANGLES, lineVertices.size(), GL_UNSIGNED_INT, 0);

}

void GLDrawer::bind()
{

}

void GLDrawer::_setVertexBufferAttriubuteLayout(GLDrawerType drawerType)
{

	if (drawerType == GLDrawerType::GLSPRITE) {

		// vertex attrubute indexes
		const int attrib_position = 0;
		const int attrib_color = 1;
		const int attrib_texture = 2;

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(attrib_position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
		glVertexAttribPointer(attrib_color, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(3 * sizeof(float)));
		glVertexAttribPointer(attrib_texture, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(7 * sizeof(float)));


	}
	else if (drawerType == GLDrawerType::GLLINE) {

		// vertex attrubute indexes
		const int attrib_position = 0;
		const int attrib_color = 1;

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(attrib_position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, 0);
		glVertexAttribPointer(attrib_color, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*)(3 * sizeof(float)));

	}


}
