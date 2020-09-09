#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <string>
#include <map>
#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include <box2d/box2d.h>

#include "texture.h"



class TextureManager
{

public:

	/*
	This is a singleton class using a local staic variable returned as a reference
	*/
	static TextureManager& instance();
	bool init();
	std::shared_ptr<sf::Texture> getTexture(std::string id);
	sf::Font getFont(std::string id);
	bool hasTexture(std::string textureId);
	void addOrReplaceTexture(std::string textureId, std::shared_ptr<sf::Texture> texture);

private:
	TextureManager();
	~TextureManager();

	std::map<std::string, std::shared_ptr<sf::Texture>> m_textureMap;
	std::map<std::string, sf::Font> m_fontMap;
	bool loadTextures();


	

};

#endif