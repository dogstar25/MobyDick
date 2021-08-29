#include "TextureManager.h"

#include <fstream>

#include <json/json.h>

#include "Renderer.h"
#include "GameConfig.h"
#include "BaseConstants.h"


TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{

	for (auto&& textureItem : m_textureMap) {

		if (textureItem.second != NULL) {

			if (textureItem.second->surface != NULL) {
				SDL_FreeSurface(textureItem.second->surface);
			}
			SDL_DestroyTexture(textureItem.second->sdlTexture);

		}
	}

	m_textureMap.clear();
	std::map<std::string, std::shared_ptr<Texture>>().swap(m_textureMap);

}


TextureManager& TextureManager::instance()
{
	static TextureManager singletonInstance;
	return singletonInstance;

}

bool TextureManager::init()
{

	//load("textureAssets");

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

void TextureManager::addOrReplaceTexture(std::string textureId, std::shared_ptr<Texture> texture)
{
	std::shared_ptr<Texture> tempTexture;

	if (hasTexture(textureId) == false)
	{
		m_textureMap.emplace(textureId, texture);
	}
	else
	{
		tempTexture = getTexture(textureId);

		SDL_FreeSurface(tempTexture->surface);
		if (tempTexture->sdlTexture != NULL) {
			SDL_DestroyTexture(tempTexture->sdlTexture);
		}

		//Use [] for the replacing of an item already there
		m_textureMap[textureId] = texture;
	}

}


bool TextureManager::load(std::string texturesAssetsFile)
{

	//Read file and stream it to a JSON object
	Json::Value root;
	std::string texturefilename = "assets/" + texturesAssetsFile + ".json";
	std::ifstream ifs(texturefilename);
	ifs >> root;

	//Get and store config values
	std::string  id, imageFilename;
	bool retainSurface = false;

	SDL_Surface* surface;
	SDL_Texture* sdlTexture;
	Texture* textureObject;

	//Loop through every texture defined in the config file, create a texture object
	//and store it in the main texture map
	for(auto itr : root["textures"])
	{
		//textureObject = new Texture();
		textureObject = new Texture();

		id = itr["id"].asString();
		imageFilename = itr["filename"].asString();
		retainSurface = itr["retainSurface"].asBool();

		//Load the file
		textureObject->surface = IMG_Load(imageFilename.c_str());

		//If this is the SDL Renderer then create the SDL texture and potentially free the image surface
		if (GameConfig::instance().rendererType() == RendererType::SDL) {
			sdlTexture = SDL_CreateTextureFromSurface(Renderer::instance().SDLRenderer(), textureObject->surface);
			textureObject->sdlTexture = sdlTexture;
			if (retainSurface == false)
			{
				SDL_FreeSurface(textureObject->surface);
			}
		}

		m_textureMap.emplace(id, std::make_shared<Texture>(*textureObject));

	}

	// Loop through every font defined and store it in the main font map
		for (auto itr : root["fonts"])
		{
			id = itr["id"].asString();
			imageFilename = itr["filename"].asString();
			m_fontMap.emplace(id, imageFilename);

		}

	return true;
}

std::string TextureManager::getFont(std::string id)
{
	std::string fontFile;

	auto iter = m_fontMap.find(id);

	if (iter != m_fontMap.end())
	{
		//fontFile = m_fontMap[id];
		fontFile = iter->second;
	}
	else //default
	{
		fontFile = m_fontMap["FONT_ARIAL_REG"];
	}

	return fontFile;
}


std::shared_ptr<Texture> TextureManager::getTexture(std::string id)
{
	std::shared_ptr<Texture> textureObject;

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










