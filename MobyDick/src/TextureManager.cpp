#include "TextureManager.h"

#include <fstream>
#include "EnumMap.h"

#include "game.h"

extern std::unique_ptr<Game> game;

TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{

	for (auto&& textureItem : m_textureMap) {

		if (textureItem.second->isRootTexture == true) {
			if (textureItem.second != NULL) {

				if (textureItem.second->surface != nullptr) {
					SDL_FreeSurface(textureItem.second->surface);
					textureItem.second->surface = nullptr;
				}
				if (textureItem.second->sdlTexture != nullptr) {
					SDL_DestroyTexture(textureItem.second->sdlTexture);
					textureItem.second->sdlTexture = nullptr;
				}

			}
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

	SDL_Texture* sdlTexture;
	Texture* textureObject;

	for (auto itr : root["textureAtlas"])
	{
		//textureObject = new Texture();
		textureObject = new Texture();

		//This is a texture object that will store the texture assets that other texture items will share
		textureObject->isRootTexture = true;

		id = itr["id"].asString();
		imageFilename = itr["filename"].asString();

		//Load the file
		textureObject->surface = IMG_Load(imageFilename.c_str());

		//If this is the SDL Renderer then create the SDL texture
		if (GameConfig::instance().rendererType() == RendererType::SDL) {
			sdlTexture = SDL_CreateTextureFromSurface(game->renderer()->sdlRenderer(), textureObject->surface);
			textureObject->sdlTexture = sdlTexture;
		}
		else if (GameConfig::instance().rendererType() == RendererType::OPENGL) {

			GL_TextureIndexType textureIndex = (GL_TextureIndexType)game->enumMap()->toEnum(itr["openglTextureIndex"].asString());

			GLuint textureAtlasId = static_cast<GLRenderer*>(game->renderer())->getTextureId(textureIndex);
			glActiveTexture((int)textureIndex);
			glBindTexture(GL_TEXTURE_2D, textureAtlasId);
			textureObject->openglTextureIndex = textureIndex;
			static_cast<GLRenderer*>(game->renderer())->prepTexture(textureObject);

		}

		m_textureMap.emplace(id, std::make_shared<Texture>(*textureObject));

	}

	//Loop through every texture defined in the config file, create a texture object
	//and store it in the main texture map
	for(auto itr : root["textureBlueprints"])
	{
		//textureObject = new Texture();
		textureObject = new Texture();

		//This is a texture object that will store the texture assets that other texture items will share
		textureObject->isRootTexture = true;

		id = itr["id"].asString();
		imageFilename = itr["filename"].asString();

		//Load the file
		textureObject->surface = IMG_Load(imageFilename.c_str());

		//If this is the SDL Renderer then create the SDL texture and potentially free the image surface
		if (GameConfig::instance().rendererType() == RendererType::SDL) {
			sdlTexture = SDL_CreateTextureFromSurface(game->renderer()->sdlRenderer(), textureObject->surface);
			textureObject->sdlTexture = sdlTexture;
		}

		m_textureMap.emplace(id, std::make_shared<Texture>(*textureObject));

	}

	//Loop through every texture atlas defined in the config file, create a texture object
	//and store it in the main texture map.
	//Texture Atlas' must be done first since hte next texture items could reference the atlas
	for (auto itr : root["textures"])
	{
		//textureObject = new Texture();
		textureObject = new Texture();

		id = itr["id"].asString();
		auto atlasId = itr["atlas"].asString();
		auto quadX = itr["quadPosition"]["x"].asInt();
		auto quadY = itr["quadPosition"]["y"].asInt();
		auto quadWidth = itr["quadPosition"]["width"].asInt();
		auto quadHeight = itr["quadPosition"]["height"].asInt();

		SDL_Rect quad = { quadX , quadY, quadWidth, quadHeight };

		textureObject->textureAtlasQuad = std::move(quad);

		if (GameConfig::instance().rendererType() == RendererType::OPENGL) {
			textureObject->openglTextureIndex = GL_TextureIndexType::MAIN_TEXTURE_ATLAS;

		}

		//Load the file
		textureObject->surface = m_textureMap[atlasId]->surface;

		//If this is the SDL Renderer then create the SDL texture and potentially free the image surface
		if (GameConfig::instance().rendererType() == RendererType::SDL) {
			textureObject->sdlTexture = m_textureMap[atlasId]->sdlTexture;
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










