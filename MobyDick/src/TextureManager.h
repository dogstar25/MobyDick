#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <string>
#include <map>
#include <memory>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#pragma warning(push,0)
#include <box2d/box2d.h>
#pragma warning(pop)

#include "texture.h"



class TextureManager
{

public:

	/*
	This is a singleton class using a local staic variable returned as a reference
	*/
	static TextureManager& instance();
	bool init(SDL_Window*);
	std::shared_ptr<Texture> getTexture(std::string id);
	std::string getFont(std::string id);
	bool hasTexture(std::string textureId);
	void addOrReplaceTexture(std::string textureId, std::shared_ptr<Texture> texture);

private:
	TextureManager();
	~TextureManager();

	std::map<std::string, std::shared_ptr<Texture>> m_textureMap;
	std::map<std::string, std::string> m_fontMap;
	bool loadTextures();


	

};

#endif