#include "TextureManager.h"

#include <fstream>

#include <json/json.h>

#include "Renderer.h"


TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{

	m_textureMap.clear();
	std::map<std::string, std::shared_ptr<sf::Texture>>().swap(m_textureMap);

}


TextureManager& TextureManager::instance()
{
	static TextureManager singletonInstance;
	return singletonInstance;

}

bool TextureManager::init()
{

	loadTextures();

	return true;
}

bool TextureManager::hasTexture(std::string textureId)
{

	if (m_textureMap.find(textureId) == m_textureMap.end())
	{
		return false;
	}
	else
	{
		return true;
	}


}

void TextureManager::addOrReplaceTexture(std::string textureId, std::shared_ptr<sf::Texture> texture)
{
	std::shared_ptr<sf::Texture> tempTexture;

	if (hasTexture(textureId) == false)
	{
		m_textureMap.emplace(textureId, texture);

	}
	else
	{
		//tempTexture = getTexture(textureId);
		//delete tempTexture;

		//Use [] for the replacing of an item already there
		m_textureMap[textureId] = texture;
	}

}


bool TextureManager::loadTextures()
{

	//Read file and stream it to a JSON object
	Json::Value root;
	std::ifstream ifs("assets/textureAssets.json");
	//ifstream ifs("assets/textureAssets_Test.json");
	ifs >> root;

	//Get and store config values
	std::string filename, id;
	sf::Texture texture;

	//Loop through every texture defined in the config file, create a texture object
	//and store it in the main texture map
	for(auto itr : root["textures"])
	{
		id = itr["id"].asString();
		filename = itr["filename"].asString();

		texture.loadFromFile(filename);

		m_textureMap.emplace(id, std::make_shared<sf::Texture>(texture));

	}

	// Loop through every font defined and store it in the main font map
		for (auto itr : root["fonts"])
		{
			id = itr["id"].asString();
			filename = itr["filename"].asString();

			sf::Font font; 
			font.loadFromFile(filename);
			m_fontMap.emplace(id, font);

		}

	return true;
}

sf::Font TextureManager::getFont(std::string id)
{
	sf::Font font;

	auto iter = m_fontMap.find(id);

	if (iter != m_fontMap.end())
	{
		//fontFile = m_fontMap[id];
		font = iter->second;
	}
	else //default
	{
		font = m_fontMap["FONT_ARIAL_REG"];
	}

	return font;
}


std::shared_ptr<sf::Texture> TextureManager::getTexture(std::string id)
{
	std::shared_ptr<sf::Texture> textureObject;

	auto iter = m_textureMap.find(id);

	if (iter != m_textureMap.end())
	{
		textureObject = iter->second;
	}
	else
	{
		textureObject = m_textureMap["TX_DEFAULT"];
	}

	return textureObject;
}










