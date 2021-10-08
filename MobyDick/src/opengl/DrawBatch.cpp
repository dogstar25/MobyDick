#include "DrawBatch.h"

#include <memory>
#include <glad/glad.h>
#include "GLRenderer.h"
#include "../game.h"
#include "SpriteVertex.h"

extern std::unique_ptr<Game> game;

DrawBatch::DrawBatch(GLDrawerType objectType, Texture* texture, GLShaderType shaderType)
{

	switch (objectType) {
		case GLDrawerType::GLSPRITE:
			m_glDrawer = static_cast<GLRenderer*>(game->renderer())->spriteDrawer();
			break;
	}

	m_shader = std::make_shared<Shader>(static_cast<GLRenderer*>(game->renderer())->shader(shaderType));

	m_texture = texture;

}

void DrawBatch::addVertexBuffer(std::vector<SpriteVertex> spriteVertices)
{

	for ( auto vertex : spriteVertices) {

		m_vertexBatch.push_back(vertex);
	}

	//index buffer 
	m_indexBufferBatch.addSpriteIndexBuffer();

}

void DrawBatch::clear()
{
	m_vertexBatch.clear();
	m_indexBufferBatch.clear();


}

void DrawBatch::draw()
{

	m_glDrawer->prepare();
	

	//SpriteVertex* arrayBufferData = (SpriteVertex*)malloc(sizeof(SpriteVertex) * 1000);
	auto test1 = sizeof(m_vertexBatch);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SpriteVertex) * 1000, nullptr, GL_DYNAMIC_DRAW);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_DEPTH_TEST);
	
	//Use the program first
	glUseProgram(m_shader->shaderProgramId());

	//Set the Projection matrix uniform
	GLuint matrixId = glGetUniformLocation(m_shader->shaderProgramId(), "u_projection_matrix");
	auto projection_matrix = static_cast<GLRenderer*>(game->renderer())->projectionMatrix();
	glUniformMatrix4fv(matrixId, 1, false, (float*)&projection_matrix);

	//Initialize the texture and set the texture uniform
	GLuint textureId = prepTexture();
	GLuint textUniformId = glGetUniformLocation(m_shader->shaderProgramId(), "u_Texture");
	//We only prepped one texture within the texture array, so we will always use index zero
	glUniform1i(textUniformId, GL_ZERO);
	//glBindTexture(GL_TEXTURE_2D, textureId);

	auto bufferSize = m_vertexBatch.size() * sizeof(SpriteVertex);


	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//SpriteVertex vertices[4];
	//vertices[0] = m_vertexBatch.at(0);
	//vertices[1] = m_vertexBatch.at(1);
	//vertices[2] = m_vertexBatch.at(2);
	//vertices[3] = m_vertexBatch.at(3);
	//auto test2 = sizeof(vertices);
	glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, &m_vertexBatch[0]);

	//Draw
	//switch (objectype) {
	//	case GLDrawerType::GLSPRITE:
	//		//
	//		// set the object type
	//		// set the vertex count
	//		// 
	//		//
	//		break;
	//}

	struct SpriteindexBuffer2 {
		glm::uvec3 topLeftTriangle{ 0,1,2 };
		glm::uvec3 botRightTriangle{ 2,3,0 };
	};

	SpriteindexBuffer2 testIndexBuffer;

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(testIndexBuffer), &testIndexBuffer, GL_DYNAMIC_DRAW);

	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

GLuint DrawBatch::prepTexture()
{

	//glEnable(GL_TEXTURE_2D);
	GLuint texture_id[1];
	SDL_Surface* surf = m_texture->surface;
	GLenum texture_format{ GL_RGB };

	auto nOfColors = surf->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
		if (surf->format->Rmask == 0x000000ff)
			texture_format = GL_RGBA;
		else
			texture_format = GL_BGRA;
	}
	else if (nOfColors == 3)     // no alpha channel
	{
		if (surf->format->Rmask == 0x000000ff)
			texture_format = GL_RGB;
		else
			texture_format = GL_BGR;
	}
	//Generate an array of textures.  We only want one texture (one element array), so trick
	//it by treating "texture" as array of length one.
	glGenTextures(1, texture_id);
	//Select (bind) the texture we just generated as the current 2D texture OpenGL is using/modifying.
	//All subsequent changes to OpenGL's texturing state for 2D textures will affect this texture.
	//glActiveTexture(GL_TEXTURE0, texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id[0]);

	//Set the minification and magnification filters.  In this case, when the texture is minified (i.e., the texture's pixels (texels) are
	//*smaller* than the screen pixels you're seeing them on, linearly filter them (i.e. blend them together).  This blends four texels for
	//each sample--which is not very much.  Mipmapping can give better results.  Find a texturing tutorial that discusses these issues
	//further.  Conversely, when the texture is magnified (i.e., the texture's texels are *larger* than the screen pixels you're seeing
	//them on), linearly filter them.  Qualitatively, this causes "blown up" (overmagnified) textures to look blurry instead of blocky.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	//load in the image data
	glTexImage2D(GL_TEXTURE_2D, 0, texture_format, surf->w, surf->h, 0, texture_format, GL_UNSIGNED_BYTE, surf->pixels);

	return texture_id[0];
}
