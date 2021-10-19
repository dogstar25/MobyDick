#include "SpriteDrawBatch.h"

#include <memory>
#include <glad/glad.h>
#include "GLRenderer.h"
#include "../game.h"
#include "SpriteVertex.h"

extern std::unique_ptr<Game> game;

SpriteDrawBatch::SpriteDrawBatch(GLDrawerType objectType, Texture* texture, GLShaderType shaderType)
{

	switch (objectType) {
	case GLDrawerType::GLSPRITE:
		m_glDrawer = static_cast<GLRenderer*>(game->renderer())->spriteDrawer();
		break;
	}

	m_shader = std::make_shared<Shader>(static_cast<GLRenderer*>(game->renderer())->shader(shaderType));

	m_texture = texture;

}

void SpriteDrawBatch::addVertexBuffer(const std::vector<SpriteVertex>& spriteVertices)
{

	for (const auto& vertex : spriteVertices) {

		m_vertexBatch.push_back(vertex);
	}

	//index buffer 
	_addSpriteIndexBuffer();

	m_objectCount++;

}

void SpriteDrawBatch::clear()
{
	m_vertexBatch.clear();
	m_indexes.clear();

	m_objectCount = 0;

}

void SpriteDrawBatch::draw()
{

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_BLEND);
	//GL_ONE_MINUS_SRC_ALPHA
	//GL_DST_ALPHA
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	m_glDrawer->prepare();


	//SpriteVertex* arrayBufferData = (SpriteVertex*)malloc(sizeof(SpriteVertex) * 1000);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SpriteVertex) * m_vertexBatch.size(), nullptr, GL_DYNAMIC_DRAW);

	//glEnable(GL_DEPTH_TEST);

	//Use the program first
	glUseProgram(m_shader->shaderProgramId());

	//Set the Projection matrix uniform
	GLuint matrixId = glGetUniformLocation(m_shader->shaderProgramId(), "u_projection_matrix");
	auto projection_matrix = static_cast<GLRenderer*>(game->renderer())->projectionMatrix();
	glUniformMatrix4fv(matrixId, 1, false, (float*)&projection_matrix);

	//Initialize the texture and set the texture uniform
	//prepTexture();
	GLuint textUniformId = glGetUniformLocation(m_shader->shaderProgramId(), "u_Texture");

	//We need to texture index here
	//int textureIndex = static_cast<GLRenderer*>(game->renderer())->addTexture(m_texture);
	//int textureIndex = static_cast<GLRenderer*>(game->renderer())->bindTexture(m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture->gLTextureId);
	//static_cast<GLRenderer*>(game->renderer())->bindTexture(m_texture);
	//glBindTexture(GL_TEXTURE_2D, [textureIndex]);
	glUniform1i(textUniformId, GL_ZERO);

	//Submit the vertices
	auto bufferSize = m_vertexBatch.size() * sizeof(SpriteVertex);
	glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, &m_vertexBatch[0]);

	//Submit the vertex indices
	auto indexBufferSize = sizeof(GL_UNSIGNED_INT) * m_indexes.size();
	//auto indexBufferSize = sizeof(glm::uint) * m_indexes.size();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, &m_indexes[0], GL_DYNAMIC_DRAW);

	glDrawElements(GL_TRIANGLES, m_indexes.size(), GL_UNSIGNED_INT, 0);

}

//void SpriteDrawBatch::prepTexture()
//{
//
//	GLuint texture_id[1];
//	SDL_Surface* surf = m_texture->surface;
//	GLenum texture_format{ GL_RGB };
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//	auto nOfColors = surf->format->BytesPerPixel;
//	if (nOfColors == 4)     // contains an alpha channel
//	{
//		if (surf->format->Rmask == 0x000000ff)
//			texture_format = GL_RGBA;
//		else
//			texture_format = GL_BGRA;
//	}
//	else if (nOfColors == 3)     // no alpha channel
//	{
//		if (surf->format->Rmask == 0x000000ff)
//			texture_format = GL_RGB;
//		else
//			texture_format = GL_BGR;
//	}
//	//Generate an array of textures.  We only want one texture (one element array), so trick
//	//it by treating "texture" as array of length one.
//	//glGenTextures(1, texture_id);
//	//Select (bind) the texture we just generated as the current 2D texture OpenGL is using/modifying.
//	//All subsequent changes to OpenGL's texturing state for 2D textures will affect this texture.
//	//glActiveTexture(GL_TEXTURE0, texture_id);
//	glBindTexture(GL_TEXTURE_2D, texture_id[0]);
//
//	//Set the minification and magnification filters.  In this case, when the texture is minified (i.e., the texture's pixels (texels) are
//	//*smaller* than the screen pixels you're seeing them on, linearly filter them (i.e. blend them together).  This blends four texels for
//	//each sample--which is not very much.  Mipmapping can give better results.  Find a texturing tutorial that discusses these issues
//	//further.  Conversely, when the texture is magnified (i.e., the texture's texels are *larger* than the screen pixels you're seeing
//	//them on), linearly filter them.  Qualitatively, this causes "blown up" (overmagnified) textures to look blurry instead of blocky.
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//
//	//load in the image data
//	glTexImage2D(GL_TEXTURE_2D, 0, texture_format, surf->w, surf->h, 0, texture_format, GL_UNSIGNED_BYTE, surf->pixels);
//
//	return;
//}

void SpriteDrawBatch::_addSpriteIndexBuffer()
{
	//Sprite Index format
	//0,1,2, top right triangle
	//2,3,0, bottom left triangle

	//Create the new set of 6 index items
	for (int i = 0; i < 6; i++) {
		m_indexes.push_back(spriteindexBuffer[i] + (m_objectCount*4));
	}
}


